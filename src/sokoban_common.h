#ifndef SOKOBAN_COMMON_H
#define SOKOBAN_COMMON_H

#include <cstddef>

namespace Sokoban
{
using stateid_t = unsigned;
using index_t   = unsigned short;
using boxhash_t = unsigned long long;

enum class Tile : unsigned char {
    None = 0, Wall, Player, PlayerOnGoal, Box, BoxOnGoal, Goal, Floor,
    Count
};

constexpr bool is_passable(Tile tile) noexcept {
    return tile != Tile::None && tile != Tile::Wall;
}

static constexpr size_t MAX_TILE_COUNT = 250;
static constexpr size_t MAX_BOX_COUNT = 10;
}

#endif
