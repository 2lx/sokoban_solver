#include "sokoban_board.h"
#include "sokoban_boxstate.h"

#include <algorithm>
#include "replace_sorted.h"
#include "string_join.h"

using namespace Sokoban;
using namespace std;

bool Board::initialize(std::vector<Tile> && maze, size_t width, size_t height) {
    _tiles = move(maze);
    _width = width;
    _height = height;
    const index_t count = static_cast<index_t>(_width * _height);

    _all_moves.set_node_count(count);
    _all_pushes.set_node_count(count);

    for (unsigned short i = 0; i < count; i++) {
        const auto tile = _tiles[i];
        if (is_passable(tile)) {
            if ( tile == Tile::Player
              || tile == Tile::PlayerOnGoal) { _player = i; }

            if ( tile == Tile::Goal
              || tile == Tile::BoxOnGoal
              || tile == Tile::PlayerOnGoal) { _goals.push_back(i); }

            if ( tile == Tile::Box
              || tile == Tile::BoxOnGoal)    { _boxes.push_back(i); }

            const bool is_topmost    = i / width == 0;
            const bool is_leftmost   = i % width == 0;
            const bool is_bottommost = i / width == height - 1;
            const bool is_rightmost  = i % width == width  - 1;

            const bool is_passableup    = !is_topmost    && is_passable(_tiles[i - width]);
            const bool is_passableleft  = !is_leftmost   && is_passable(_tiles[i - 1]);
            const bool is_passableright = !is_rightmost  && is_passable(_tiles[i + 1]);
            const bool is_passabledown  = !is_bottommost && is_passable(_tiles[i + width]);

            // insert reversed edges
            if (is_passableup && is_passabledown) {
                _all_pushes.add_edge(static_cast<index_t>(i - width), i);
                _all_pushes.add_edge(static_cast<index_t>(i + width), i);
            }
            if (is_passableleft && is_passableright) {
                _all_pushes.add_edge(static_cast<index_t>(i - 1), i);
                _all_pushes.add_edge(static_cast<index_t>(i + 1), i);
            }

            if (is_passableup)    { _all_moves.add_edge(i, static_cast<index_t>(i - width)); }
            if (is_passableleft)  { _all_moves.add_edge(i, static_cast<index_t>(i - 1)); }
            if (is_passableright) { _all_moves.add_edge(i, static_cast<index_t>(i + 1)); }
            if (is_passabledown)  { _all_moves.add_edge(i, static_cast<index_t>(i + width)); }
        }
    }

    _all_pushes.remove_impassable(_goals);
    _all_pushes.transpose();

    update_boxdep_moves();

    return _boxes.size() == _goals.size();
}

void Board::print_graphs() const {
    cout << "Possible moves:\n";
    _all_moves.print();
    cout << endl;

    cout << "Possible pushes:\n";
    _all_pushes.print();
}

BoxState Board::current_state() {
    BoxState state;

    state.player_position = _boxdep_moves.find_adjacent_min(_player);
    copy(begin(_boxes), end(_boxes), begin(state.box_positions));

    return state;
}

void Board::update_boxdep_moves() {
    _boxdep_moves = _all_moves;
    for (const auto b: _boxes) {
        _boxdep_moves.remove_vertice_edges(b);
    }
    _cur_moves = move(_boxdep_moves.get_joined(_player));
    sort(begin(_cur_moves), end(_cur_moves));
}

void Board::set_boxstate_and_push(const BoxState & bs, PushInfo pt) {
    copy_n(begin(bs.box_positions), _boxes.size(), begin(_boxes));
    replace_sorted(begin(_boxes), end(_boxes), pt.from(), pt.to());
    _player = pt.from();
    update_boxdep_moves();
}

void Board::set_boxstate(const BoxState & bs) {
    copy_n(begin(bs.box_positions), _boxes.size(), begin(_boxes));
    _player = bs.player_position;
    update_boxdep_moves();
}

bool Board::is_complete() const {
    return _boxes == _goals;
}

bool Board::has_deadlock(index_t ind) const {
    auto fn_has_box = [=](size_t ind) {
        return binary_search(begin(_boxes), end(_boxes), ind);
    };
    const bool has_box_u    = fn_has_box(ind - _width);
    const bool has_box_l  = fn_has_box(ind - 1);
    const bool has_box_r = fn_has_box(ind + 1);
    const bool has_box_d  = fn_has_box(ind + _width);

    const bool has_box_ul = fn_has_box(ind - _width - 1);
    const bool has_box_ur = fn_has_box(ind - _width + 1);
    const bool has_box_dl = fn_has_box(ind + _width - 1);
    const bool has_box_dr = fn_has_box(ind + _width + 1);

    const bool not_goal_u = !binary_search(begin(_goals), end(_goals), ind - _width);
    const bool not_goal_l = !binary_search(begin(_goals), end(_goals), ind - 1);
    const bool not_goal_r = !binary_search(begin(_goals), end(_goals), ind + 1);
    const bool not_goal_d = !binary_search(begin(_goals), end(_goals), ind + _width);

    const bool not_goal_ul = !binary_search(begin(_goals), end(_goals), ind - _width - 1);
    const bool not_goal_ur = !binary_search(begin(_goals), end(_goals), ind - _width + 1);
    const bool not_goal_dl = !binary_search(begin(_goals), end(_goals), ind + _width - 1);
    const bool not_goal_dr = !binary_search(begin(_goals), end(_goals), ind + _width + 1);

    const bool wall_lu = _tiles[ind - _width - 1] == Tile::Wall;
    const bool wall_u  = _tiles[ind - _width]     == Tile::Wall;
    const bool wall_ru = _tiles[ind - _width + 1] == Tile::Wall;
    const bool wall_l  = _tiles[ind          - 1] == Tile::Wall;
    const bool wall_r  = _tiles[ind          + 1] == Tile::Wall;
    const bool wall_ld = _tiles[ind + _width - 1] == Tile::Wall;
    const bool wall_d  = _tiles[ind + _width]     == Tile::Wall;
    const bool wall_rd = _tiles[ind + _width + 1] == Tile::Wall;

    const bool not_goal_dest = !binary_search(begin(_goals), end(_goals), ind);

    const bool check_up    = has_box_u && (not_goal_dest || !not_goal_u);
    const bool check_left  = has_box_l && (not_goal_dest || !not_goal_l);
    const bool check_right = has_box_r && (not_goal_dest || !not_goal_r);
    const bool check_down  = has_box_d && (not_goal_dest || !not_goal_d);

    if (
           ( has_box_u  && has_box_ul &&  has_box_l &&
           (not_goal_u || not_goal_ul || not_goal_l || not_goal_dest)) ||
           ( has_box_u &&  has_box_ur &&  has_box_r &&
           (not_goal_u || not_goal_ur || not_goal_r || not_goal_dest)) ||
           ( has_box_d &&  has_box_dl &&  has_box_l &&
           (not_goal_d || not_goal_dl || not_goal_l || not_goal_dest)) ||
           ( has_box_d &&  has_box_dr &&  has_box_r &&
           (not_goal_d || not_goal_dr || not_goal_r || not_goal_dest))
            ) {
        return true;
    }

    if (   (check_up    && wall_lu && wall_l)
        || (check_up    && wall_ru && wall_r)
        || (check_left  && wall_lu && wall_u)
        || (check_left  && wall_ld && wall_d)
        || (check_right && wall_ru && wall_u)
        || (check_right && wall_rd && wall_d)
        || (check_down  && wall_ld && wall_l)
        || (check_down  && wall_rd && wall_r)) {
        return true;
    }
    return false;
}


bool Board::has_deadlock(const PushInfo & pi) const {
    index_t ibox      = pi.from();
    index_t ibox_dest = pi.to();

    auto fn_has_box = [=](size_t ind) {
        return (ibox != ind) && binary_search(begin(_boxes), end(_boxes), ind);
    };
    const bool has_box_u  = fn_has_box(ibox_dest - _width);
    const bool has_box_l  = fn_has_box(ibox_dest - 1);
    const bool has_box_r  = fn_has_box(ibox_dest + 1);
    const bool has_box_d  = fn_has_box(ibox_dest + _width);

    const bool has_box_ul = fn_has_box(ibox_dest - _width - 1);
    const bool has_box_ur = fn_has_box(ibox_dest - _width + 1);
    const bool has_box_dl = fn_has_box(ibox_dest + _width - 1);
    const bool has_box_dr = fn_has_box(ibox_dest + _width + 1);

    const bool not_goal_u  = !binary_search(begin(_goals), end(_goals), ibox_dest - _width);
    const bool not_goal_l  = !binary_search(begin(_goals), end(_goals), ibox_dest - 1);
    const bool not_goal_r  = !binary_search(begin(_goals), end(_goals), ibox_dest + 1);
    const bool not_goal_d  = !binary_search(begin(_goals), end(_goals), ibox_dest + _width);

    const bool not_goal_ul = !binary_search(begin(_goals), end(_goals), ibox_dest - _width - 1);
    const bool not_goal_ur = !binary_search(begin(_goals), end(_goals), ibox_dest - _width + 1);
    const bool not_goal_dl = !binary_search(begin(_goals), end(_goals), ibox_dest + _width - 1);
    const bool not_goal_dr = !binary_search(begin(_goals), end(_goals), ibox_dest + _width + 1);

    const bool wall_lu = _tiles[ibox_dest - _width - 1] == Tile::Wall;
    const bool wall_u  = _tiles[ibox_dest - _width]     == Tile::Wall;
    const bool wall_ru = _tiles[ibox_dest - _width + 1] == Tile::Wall;
    const bool wall_l  = _tiles[ibox_dest          - 1] == Tile::Wall;
    const bool wall_r  = _tiles[ibox_dest          + 1] == Tile::Wall;
    const bool wall_ld = _tiles[ibox_dest + _width - 1] == Tile::Wall;
    const bool wall_d  = _tiles[ibox_dest + _width]     == Tile::Wall;
    const bool wall_rd = _tiles[ibox_dest + _width + 1] == Tile::Wall;

    const bool not_goal_dest = !binary_search(begin(_goals), end(_goals), ibox_dest);

    const bool check_up    = has_box_u && (not_goal_dest || not_goal_u);
    const bool check_left  = has_box_l && (not_goal_dest || not_goal_l);
    const bool check_right = has_box_r && (not_goal_dest || not_goal_r);
    const bool check_down  = has_box_d && (not_goal_dest || not_goal_d);

    if (
           ( has_box_u  && has_box_ul &&  has_box_l &&
           (not_goal_u || not_goal_ul || not_goal_l || not_goal_dest)) ||
           ( has_box_u &&  has_box_ur &&  has_box_r &&
           (not_goal_u || not_goal_ur || not_goal_r || not_goal_dest)) ||
           ( has_box_d &&  has_box_dl &&  has_box_l &&
           (not_goal_d || not_goal_dl || not_goal_l || not_goal_dest)) ||
           ( has_box_d &&  has_box_dr &&  has_box_r &&
           (not_goal_d || not_goal_dr || not_goal_r || not_goal_dest))
            ) {
        return true;
    }

    if (   (check_up    && wall_lu && wall_l)
        || (check_up    && wall_ru && wall_r)
        || (check_left  && wall_lu && wall_u)
        || (check_left  && wall_ld && wall_d)
        || (check_right && wall_ru && wall_u)
        || (check_right && wall_rd && wall_d)
        || (check_down  && wall_ld && wall_l)
        || (check_down  && wall_rd && wall_r)) {
        return true;
    }
    return false;
}

vector<PushInfo> Board::possible_pushes() const {
    vector<PushInfo> result;

    for (const auto ibox: _boxes) {
        for (auto ibox_dest: _all_pushes.siblings(ibox)) {
            if (ibox_dest == decltype(_all_pushes)::EMPTY) { continue; }
            index_t iplayer_dest;

            // we can omit the check of iplayer_dest correctness, because
            // it was checked during the initialize of the '_all_pushes' graph.
            // the formula is equivalent to: ibox + (ibox - ibox_dest)
            iplayer_dest = static_cast<index_t>((ibox << 1) - ibox_dest);

            // check if the destination of the pushed box is occupied
            // or player's place is occupied
            if (binary_search(begin(_boxes), end(_boxes), ibox_dest)) { continue; }
            if (binary_search(begin(_boxes), end(_boxes), iplayer_dest)) { continue; }

            // check if player has access to the cell to push
            if (!binary_search(begin(_cur_moves), end(_cur_moves), iplayer_dest)) { continue; }

            if (has_deadlock(PushInfo(ibox, ibox_dest))) { continue; }

            result.emplace_back(ibox, ibox_dest);
        }
    }

    return result;
}
