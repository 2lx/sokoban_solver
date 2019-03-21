#ifndef SOKOBAN_BOXSTATE_H
#define SOKOBAN_BOXSTATE_H

#include "sokoban_common.h"
#include "zobrist_hash.h"

#include <cstddef>
#include <array>
#include <bitset>

namespace Sokoban
{
struct BoxState {
    std::array<index_t, MAX_BOX_COUNT> box_indexes;
    index_t player_index;
    std::bitset<MAX_TILE_COUNT> box_bits;
    stateid_t unique_index;

    static size_t box_count;
    static const ZobristHash<MAX_TILE_COUNT, boxhash_t> zhash;

public:
    BoxState() : box_indexes{}, player_index{ 0 }, box_bits{ 0 }, unique_index{ 0 } {
        for (auto & bp: box_indexes) { bp = 0; }
    }

    static void set_box_count(size_t bcount) { box_count = bcount; }
    boxhash_t hash() const;
};

inline bool operator == (const BoxState & l, const BoxState & r) {
    return l.box_bits     == r.box_bits
        && l.player_index == r.player_index;
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
