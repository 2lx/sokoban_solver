#ifndef SOKOBAN_BOARD_H
#define SOKOBAN_BOARD_H

#include <vector>
#include "sokoban_common.h"
#include "sparse_graph.h"

namespace Sokoban
{
class BoxState;

class Board {
    SparseGraph<false> gmoves;
    SparseGraph<true> gpushes;

    unsigned short player;
    std::vector<unsigned short> goals;
    std::vector<unsigned short> boxes;
    std::vector<Tile> tiles;
    unsigned short width, height;

public:
    Board() = default;
    void initialize(std::vector<Tile> && maze,
                    unsigned short width, unsigned short height);

    BoxState current_state();
    void push(PushType pt);

    void set_boxstate(const BoxState & bs);
    std::vector<PushType> possible_pushes() const;

    bool is_complete() const;

    void print_graphs() const;
};

}

#endif
