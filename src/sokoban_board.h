#ifndef SOKOBAN_BOARD_H
#define SOKOBAN_BOARD_H

#include "sokoban_common.h"
#include "sokoban_board_state.h"
#include "sokoban_board_graphs.h"
#include "sokoban_deadlock_tester.h"

#include <vector>
#include <bitset>
#include <string>

namespace Sokoban
{
class BoxState;
class PushInfo;

class Board {
    BoardState     _state;
    BoardGraphs    _graphs;
    DeadlockTester _dltester;

public:
    struct StateStats {
        size_t boxes_on_goals_count;
        size_t ordered_boxes_on_goals_count;
        std::pair<size_t, size_t> push_distances;
    };

    Board() = default;

    Board(const Board &) = delete;
    Board(Board &&) = delete;
    Board & operator=(const Board &) = delete;
    Board & operator=(Board &&) = delete;

    bool initialize(std::vector<Tile> && maze, size_t w, size_t h);
    void print_information() const;

    size_t box_count() const { return _state.box_count(); }

    BoxState current_state() const;
    void set_boxstate(const BoxState & bs);
    void set_boxstate_and_push(const BoxState & bs, const PushInfo & pi);

    bool is_complete() const { return _state.is_complete(); }

    std::vector<std::pair<PushInfo, StateStats>> possible_pushes();

    void print_state() const { _state.print(); }
    void print_graphs() const;
};
}

#endif
