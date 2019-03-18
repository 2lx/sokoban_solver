#include "sokoban_boxstate.h"

using namespace Sokoban;

size_t BoxState::box_count;
const ZobristHash<MAX_TILE_COUNT, boxhash_t> BoxState::zhash = {};

boxhash_t BoxState::hash() const {
    auto result = zhash.hash<>(box_positions, box_count);
    result ^= zhash.hash(player_position);

    return result;
}
