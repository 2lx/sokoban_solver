#include "sokoban_board.h"
#include "sokoban_boxstate.h"
#include "sokoban_formatter.h"

#include <algorithm>
#include "replace_sorted.h"
#include "string_join.h"
#include <iostream>

using namespace Sokoban;
using namespace std;

bool Board::initialize(std::vector<Tile> && maze, size_t width, size_t height) {
    _tiles = move(maze);
    _width = width;
    _height = height;

    if (!initialize_indexes()) return false;
    if (!initialize_graphs())  return false;
    if (!initialize_checks())  return false;
    return true;
}

bool Board::initialize_indexes() {
    _is_wall.resize(_tiles.size());
    _is_goal.resize(_tiles.size());
    _is_box.resize(_tiles.size());

    for (index_t i = 0; i < _tiles.size(); i++) {
        const auto & tile = _tiles[i];

        if (   tile == Tile::Wall
            || tile == Tile::None) { _is_wall[i] = true; continue; }

        if (   tile == Tile::Player
            || tile == Tile::PlayerOnGoal) { _player = i; }

        if (   tile == Tile::Goal
            || tile == Tile::BoxOnGoal
            || tile == Tile::PlayerOnGoal)
        {
            _goals.push_back(i);
            _is_goal[i] = true;
        }

        if (   tile == Tile::Box
            || tile == Tile::BoxOnGoal)
        {
            _boxes.push_back(i);
            _is_box[i] = true;
        }
    }
    return _boxes.size() == _goals.size();
}

bool Board::initialize_graphs() {
    const size_t count = _width * _height;
    _all_moves.resize(count);
    _all_pushes.resize(count);

    for (index_t i = 0; i < count; i++) {
        if (_is_wall[i]) { continue; }

        const index_t ind_u = static_cast<index_t>(i - _width);
        const index_t ind_l = static_cast<index_t>(i - 1u);
        const index_t ind_r = static_cast<index_t>(i + 1u);
        const index_t ind_d = static_cast<index_t>(i + _width);

        const bool is_passable_u = (i / _width != 0)           && !_is_wall[ind_u];
        const bool is_passable_l = (i % _width != 0)           && !_is_wall[ind_l];
        const bool is_passable_r = (i % _width != _width  - 1) && !_is_wall[ind_r];
        const bool is_passable_d = (i / _width != _height - 1) && !_is_wall[ind_d];

        // insert reversed edges
        if (is_passable_u && is_passable_d) {
            _all_pushes.insert_edge(ind_u, i);
            _all_pushes.insert_edge(ind_d, i);
        }
        if (is_passable_l && is_passable_r) {
            _all_pushes.insert_edge(ind_l, i);
            _all_pushes.insert_edge(ind_r, i);
        }

        if (is_passable_u) { _all_moves.insert_edge(i, ind_u); }
        if (is_passable_l) { _all_moves.insert_edge(i, ind_l); }
        if (is_passable_r) { _all_moves.insert_edge(i, ind_r); }
        if (is_passable_d) { _all_moves.insert_edge(i, ind_d); }
    }

    /* for (const auto goali: _goals) { */
    /*     auto passable_boxes = _all_pushes.check_if_passable(goali, _boxes); */
    /* } */

    _all_pushes.remove_impassable(_goals);
    _all_pushes.transpose();
    update_boxdep_moves();

    return true;
}

bool Board::initialize_checks() {
    _checks.resize(_tiles.size());

    auto symmetric_index =
                [width = static_cast<int>(_width)]
                (size_t ind, int vdiff, int hdiff, bool vreflect, bool hreflect) {
        const int v = (vreflect ? -width : width);
        const int h = (hreflect ? -1 : 1);

        return static_cast<index_t>(ind + v*vdiff + h*hdiff);
    };

    auto check1 = [&](const index_t i1) {
        return _is_box[i1];
    };

    auto check3 = [&](const index_t i1, const index_t i2, const index_t i3) {
        return _is_box[i1] && _is_box[i2] && _is_box[i3];
    };

    array<pair<bool, bool>, 4> reflections = {{
        {false, false}, {true, false}, {false, true}, {true, true}
    }};

    for (index_t ind = 0; ind < _tiles.size(); ind++) {
        if (_is_wall[ind]) { continue; }

        /* bool vrefl = false, hrefl = false; */
        for (const auto [vrefl, hrefl]: reflections) {
            const index_t ind_ul = symmetric_index(ind, -1, -1, vrefl, hrefl);
            const index_t ind_u  = symmetric_index(ind, -1,  0, vrefl, hrefl);
            /* const index_t ind_ur = symmetric_index(i, -1,  1, vrefl, hrefl); */
            const index_t ind_l  = symmetric_index(ind,  0, -1, vrefl, hrefl);
            /* const index_t ind_r  = symmetric_index(i, -1,  1, vrefl, hrefl); */
            /* const index_t ind_dl = symmetric_index(i, -1, -1, vrefl, hrefl); */
            /* const index_t ind_d  = symmetric_index(i, -1,  0, vrefl, hrefl); */
            /* const index_t ind_dr = symmetric_index(i, -1,  1, vrefl, hrefl); */

            if (_boxes.size() < 2) { continue; }
            // check if there are two walls, and there is a box on the left
            // ##
            // $?
            if (_is_wall[ind_ul] && _is_wall[ind_u] && !_is_wall[ind_l]
                && (!_is_goal[ind] || !_is_goal[ind_l])) {
                _checks[ind].push_back(bind(check1, ind_l));
            }
            // #$
            // #?
            if (_is_wall[ind_ul] && _is_wall[ind_l] && !_is_wall[ind_u]
                && (!_is_goal[ind] || !_is_goal[ind_u])) {
                _checks[ind].push_back(bind(check1, ind_u));
            }

            if (_boxes.size() < 4) { continue; }
            // check if box is in square with no walls, and there are three other boxes,
            // and there is at least one of them is not on a goal
            // $$
            // $?
            if (    !_is_wall[ind_ul] && !_is_wall[ind_u] && !_is_wall[ind_l]
                && (!_is_goal[ind_ul] || !_is_goal[ind_u] || !_is_goal[ind_l] || !_is_goal[ind])) {
                _checks[ind].push_back(bind(check3, ind_ul, ind_u, ind_l));
            }

        }
    }
    return true;
}

bool Board::check_deadlocks(index_t ind) const {
    for (auto & fn: _checks[ind]) {
        if (fn()) {
            /* print_state({ ind }); */
            return true;
        }
    }
    return false;
}

string Board::level_as_string() const {
    string result(_tiles.size() + 1, ' ');

    for (size_t i = 0; i < _tiles.size(); ++i) {
        const Tile tile = get_tile(_is_wall[i], _player == i, _is_box[i], _is_goal[i]);
        result[i] = Formatter::decode(tile);
    }
    return result;
}

void Board::print_state(const vector<index_t> & marked) const {
    string level = level_as_string();
    for_each(begin(marked), end(marked), [&level](index_t ind){ level[ind] = '?'; });

    auto it = level.cbegin();
    for (size_t i = 0u; i < _height; ++i) {
        copy_n(it, _width, ostream_iterator<char>(cout));
        cout << '\n';
        advance(it, _width);
    }
    cout << endl;
}

void Board::print_graphs() const {
    /* cout << "Possible moves:\n"; */
    /* _all_moves.print(); */
    /* cout << endl; */
    /*  */
    /* cout << "Possible pushes:\n"; */
    /* _all_pushes.print(); */
}

BoxState Board::current_state() {
    BoxState state;

    auto nodes = _boxdep_moves.nodes();
    index_t min = MAX_TILE_COUNT;

    // we don't use min_element because the graph iterator has the input_iterator category,
    // so we can't read a value twice
    for_each (nodes.begin(_player), nodes.end(),
              [&min](const auto ind){ if (min > ind) { min = ind; } });
    state.player_position = min;

    copy(begin(_boxes), end(_boxes), begin(state.box_positions));

    return state;
}

void Board::update_boxdep_moves() {
    _boxdep_moves = _all_moves;
    for (const auto b: _boxes) {
        _boxdep_moves.remove_node(b);
    }
}

void Board::set_boxstate_and_push(const BoxState & bs, const PushInfo & pt) {
    for (auto bi: _boxes) { _is_box[bi] = false; }

    copy_n(begin(bs.box_positions), _boxes.size(), begin(_boxes));
    replace_sorted(begin(_boxes), end(_boxes), pt.from(), pt.to());
    for (auto bi: _boxes) { _is_box[bi] = true; }

    _player = pt.from();
    update_boxdep_moves();
}

void Board::set_boxstate(const BoxState & bs) {
    for (auto bi: _boxes) { _is_box[bi] = false; }
    copy_n(begin(bs.box_positions), _boxes.size(), begin(_boxes));
    for (auto bi: _boxes) { _is_box[bi] = true; }

    _player = bs.player_position;
    update_boxdep_moves();
}

bool Board::is_complete() const {
    return _boxes == _goals;
}

vector<PushInfo> Board::possible_pushes() {
    vector<PushInfo> result;

    // calculate all tile indexes which available for player with current boxes
    // and player positions
    vector<bool> valid_moves(_tiles.size(), false);
    auto nodes = _boxdep_moves.nodes();
    for_each(nodes.begin(_player), nodes.end(),
             [&valid_moves](auto ind){ valid_moves[ind] = true; });

    for (const auto ibox: _boxes) {
        _is_box[ibox] = false;
        for (auto it = _all_pushes.edges_begin(ibox); it != _all_pushes.edges_end(); ++it) {
            index_t ibox_dest = *it;

            // we can omit the check of iplayer_dest correctness, because
            // it was checked during the initialize of the '_all_pushes' graph.
            // the formula is equivalent to: ibox + (ibox - ibox_dest)
            auto iplayer_dest = static_cast<index_t>((ibox << 1) - ibox_dest);

            // check if the destination of the pushed box is occupied
            // or player's place is occupied
            if (_is_box[ibox_dest] || _is_box[iplayer_dest]) { continue; }

            // check if player has access to the cell to push
            if (!valid_moves[iplayer_dest]) { continue; }

            // check for deadlocks
            if (check_deadlocks(ibox_dest)) { continue; }

            result.emplace_back(ibox, ibox_dest);
        }
        _is_box[ibox] = true;
    }

    return result;
}
