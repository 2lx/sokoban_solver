#ifndef SOKOBAN_BOXSTATE_H
#define SOKOBAN_BOXSTATE_H

#include <cstddef>
#include <array>
#include "sokoban_common.h"
#include "zobrist_hash.h"

namespace Sokoban
{
struct BoxState {
    std::array<index_t, MAX_BOX_COUNT> box_positions;
    index_t player_position;
    stateid_t unique_index;

    static size_t box_count;
    static const ZobristHash<MAX_TILE_COUNT, boxhash_t> zhash;

public:
    BoxState() : box_positions{}, player_position{ 0 }, unique_index{ 0 } {
        for (auto & bp: box_positions) { bp = 0; }
    }

    boxhash_t hash() const;
};

inline bool operator == (const BoxState & l, const BoxState & r) {
    return l.player_position == r.player_position
        && l.box_positions   == r.box_positions;
}

}

namespace std {
template <> struct hash<Sokoban::BoxState> {
    typedef Sokoban::BoxState argument_type;
    typedef Sokoban::boxhash_t result_type;

    result_type operator()(argument_type const & bs) const noexcept {
        return bs.hash();
    }
};
}

#endif
