#include "sokoban_board.h"
#include "sokoban_boxstate.h"
#include "sokoban_pushinfo.h"

#include "string_join.h"

#include <algorithm>
#include <iostream>

using namespace Sokoban;
using namespace std;

bool Board::initialize(std::vector<Tile> && maze, size_t width, size_t height) {
    assert(maze.size() <= MAX_TILE_COUNT);

    if (!_state.initialize(move(maze), width, height)) return false;
    if (!_graphs.initialize(_state))   return false;
    if (!_dltester.initialize(_state)) return false;

    // print states
    cout << "LEVEL: " << endl;
    _state.print();
    print_graphs();

    return true;
}

BoxState Board::current_state() const {
    BoxState bs = _state.current_boxstate();
    bs.player_index = _graphs.min_move_index(bs.player_index);

    return bs;
}

void Board::set_boxstate_and_push(const BoxState & bs, const PushInfo & pi) {
    _state.set_boxstate(bs);
    _state.apply_push(pi);

    _graphs.narrow_moves(_state.box_indexes());
}

void Board::set_boxstate(const BoxState & bs) {
    _state.set_boxstate(bs);

    _graphs.narrow_moves(_state.box_indexes());
}

vector<PushInfo> Board::possible_pushes() {
    vector<PushInfo> result;

    // calculate all tile indexes which available for player with current boxes
    // and player positions
    auto valid_moves = _graphs.narrowed_moves_bitset(_state.player());
    for (size_t i = 0; i < _state.box_count(); ++i) {
        const auto ibox = _state.box_indexes()[i];
        _state.remove_bitset_box(ibox);

        const auto & groute = _graphs.route(i);
        for (auto it = groute.edges_begin(ibox); it != groute.edges_end(); ++it) {
            index_t ibox_dest = *it;

            // we can omit the check of iplayer_dest correctness, because
            // it was checked during the initialize of the '_all_pushes' graph.
            // the formula is equivalent to: ibox + (ibox - ibox_dest)
            auto iplayer_dest = static_cast<index_t>((ibox << 1) - ibox_dest);

            // check if the destination of the pushed box is occupied
            // or player's place is occupied
            if (_state.is_box(ibox_dest) || _state.is_box(iplayer_dest)) { continue; }

            // check if player has access to the cell to push
            if (!valid_moves[iplayer_dest]) { continue; }

            // check if the new combination of boxes is in a deadlock state
            if (_dltester.test_for_index(ibox_dest)) { continue; }

            result.emplace_back(ibox, ibox_dest);
        }
        _state.recover_bitset_box(ibox);
    }

    return result;
}

void Board::print_graphs() const {
    for (size_t i = 0; i < _state.box_count(); ++i) {
        const index_t boxi = _state.box_indexes()[i];
        vector<index_t> pushes;

        auto nodes = _graphs.route(i).nodes();
        for_each (nodes.begin(boxi), nodes.end(),
                [&pushes](auto ind){ pushes.push_back(ind); });

        cout << "BOX[" << i << "]=" << boxi
             << ": GOALS: " << string_join(_graphs.goals(i), ",")
             << "; PUSHES: " << endl;
        _state.print(pushes);
    }
}

