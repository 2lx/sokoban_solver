#ifndef SOKOBAN_BOARD_H
#define SOKOBAN_BOARD_H

#include <vector>
#include "sokoban_common.h"
#include "sokoban_pushinfo.h"
#include "sparse_graph.h"

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
    SparseGraph<index_t, DIR_COUNT, false>  _boxdep_moves;

    void update_boxdep_moves();
public:
    Board() = default;
    bool initialize(std::vector<Tile> && maze, size_t w, size_t h);

    size_t box_count() const { return _boxes.size(); }

    BoxState current_state();
    void set_boxstate(const BoxState & bs);
    void set_boxstate_and_push(const BoxState & bs, PushInfo pt);

    std::vector<PushInfo> possible_pushes() const;
    bool has_deadlock(const PushInfo & pi) const;
    bool has_deadlock(index_t ind) const;

    bool is_complete() const;

    void print_graphs() const;
};

}

#endif
