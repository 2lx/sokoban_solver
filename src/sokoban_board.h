#ifndef SOKOBAN_BOARD_H
#define SOKOBAN_BOARD_H

#include "sokoban_common.h"
#include "sokoban_pushinfo.h"
#include "sparse_graph.h"

#include <vector>
#include <functional>
#include <string>

namespace Sokoban
{
class BoxState;

class Board {
    std::vector<Tile> _tiles;
    size_t _width, _height;

    index_t _player;
    std::vector<index_t> _goals;
    std::vector<index_t> _boxes;
    SparseGraph<index_t, DIR_COUNT, false> _all_moves;
    SparseGraph<index_t, DIR_COUNT, true>  _all_pushes;
    SparseGraph<index_t, DIR_COUNT, false> _boxdep_moves;

    std::vector<std::vector<std::function<bool()>>> _checks;
    std::vector<bool> _is_wall, _is_goal, _is_box;

    std::vector<std::vector<index_t>> _boxes_goals;

    void update_boxdep_moves();

    bool initialize_indexes();
    bool initialize_graphs();
    bool initialize_checks();

    bool check_deadlocks(index_t ind) const;

    std::string level_as_string() const;
    void print_state(const std::vector<index_t> & marked = {}) const;

public:
    Board() = default;

    Board(const Board &) = delete;
    Board(Board &&) = delete;
    Board & operator=(const Board &) = delete;
    Board & operator=(Board &&) = delete;

    bool initialize(std::vector<Tile> && maze, size_t w, size_t h);

    size_t box_count() const { return _boxes.size(); }

    BoxState current_state();
    void set_boxstate(const BoxState & bs);
    void set_boxstate_and_push(const BoxState & bs, const PushInfo & pt);

    std::vector<PushInfo> possible_pushes();
    bool is_complete() const;

    void print_graphs() const;
};

}

#endif
