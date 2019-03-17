#ifndef SOKOBAN_COMMON_H
#define SOKOBAN_COMMON_H

#include <cstddef>
#include <tuple>

namespace Sokoban
{
using uchar  = unsigned char;
using uint   = unsigned int;
using ushort = unsigned short int;

using stateid_t = unsigned int;
using index_t = unsigned short;

static constexpr size_t MAX_TILE_COUNT = 250;
static constexpr size_t MAX_BOX_COUNT = 10;

enum class Tile : uchar {
    None = 0, Wall, Player, PlayerOnGoal, Box, BoxOnGoal, Goal, Floor,
    TileCount
};

enum class Direction : uchar {
    Up, Left, Right, Down
};

constexpr char to_char(const Direction d) noexcept {
    switch (d) {
        case Direction::Up:    return 'U';
        case Direction::Left:  return 'L';
        case Direction::Right: return 'R';
        default:               return 'D';
    }
}

constexpr bool is_passable(Tile tile) noexcept {
    return tile != Tile::None && tile != Tile::Wall;
}

using PushType = std::tuple<index_t, index_t, Direction>;

}

#endif
