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

constexpr Tile get_tile(bool is_wall, bool is_player, bool is_box, bool is_goal) {
    if (is_wall)              return Tile::Wall;
    if (is_player && is_goal) return Tile::PlayerOnGoal;
    if (is_player)            return Tile::Player;
    if (is_box && is_goal)    return Tile::BoxOnGoal;
    if (is_box)               return Tile::Box;
    if (is_goal)              return Tile::Goal;
    return Tile::Floor;
}

static constexpr size_t MAX_TILE_COUNT = 250;
static constexpr size_t MAX_BOX_COUNT = 10;
}

#endif
