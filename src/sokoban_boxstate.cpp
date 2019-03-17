#include "sokoban_boxstate.h"

using namespace Sokoban;

size_t BoxState::box_count;
const ZobristHash<MAX_TILE_COUNT> BoxState::zhash = {};

unsigned long long BoxState::hash() const {
    auto result = zhash.hash<ushort>(box_positions, box_count);
    result ^= zhash.hash(player_position);

    return result;
}
