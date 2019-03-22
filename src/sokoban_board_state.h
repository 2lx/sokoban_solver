#ifndef SOKOBAN_BOARD_STATE_H
#define SOKOBAN_BOARD_STATE_H

#include "sokoban_common.h"
#include <vector>

namespace Sokoban
{
class PushInfo;
class BoxState;

class BoardState {
    std::vector<Tile> _tiles;
    size_t _width, _height;

    index_t _player;
    std::vector<index_t> _goals, _boxes;
    flags _is_wall, _is_goal, _is_box;

    std::string level_as_string(bool draw_boxes) const;
    void print_level_string(const std::string & level) const;

public:
    BoardState() = default;

    BoardState(const BoardState &) = delete;
    BoardState(BoardState &&) = delete;
    BoardState & operator=(const BoardState &) = delete;
    BoardState & operator=(BoardState &&) = delete;

    bool initialize(std::vector<Tile> && tiles, size_t width, size_t height);
    void print(const std::vector<index_t> & marked = {}) const;
    void print_distances(const std::vector<size_t> & distances) const;

    BoxState current_boxstate() const;
    void set_boxstate(const BoxState & bs);
    void apply_push(const PushInfo & pi);

    bool is_complete() const  { return _is_box == _is_goal; }

    size_t tile_count() const { return _tiles.size(); }
    size_t box_count()  const { return _boxes.size(); }
    size_t width()  const     { return _width;  }
    size_t height() const     { return _height; }

    index_t player() const                            { return _player; }
    const std::vector<index_t> & box_indexes()  const { return _boxes; }
    const std::vector<index_t> & goal_indexes() const { return _goals; }
    index_t box_index(size_t index)  const { return _boxes[index]; }
    index_t goal_index(size_t index) const { return _goals[index]; }

    void remove_bitset_box(const index_t index)  { _is_box[index] = false; }
    void recover_bitset_box(const index_t index) { _is_box[index] = true;  }

    bool is_wall(const size_t index) const     { return _is_wall[index]; }
    bool is_goal(const size_t index) const     { return _is_goal[index]; }
    bool is_box (const size_t index) const     { return _is_box[index];  }
};
}

#endif
