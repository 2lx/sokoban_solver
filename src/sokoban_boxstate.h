#ifndef SOKOBAN_BOXSTATE_H
#define SOKOBAN_BOXSTATE_H

#include <cstddef>
#include <array>
#include "sokoban_common.h"
#include "zobrist_hash.h"

namespace Sokoban
{
struct BoxState {
    std::array<ushort, MAX_BOX_COUNT> box_positions;
    ushort player_position;
    unsigned unique_index;

    static size_t box_count;
    static const ZobristHash<MAX_TILE_COUNT> zhash;

public:
    BoxState() : box_positions{}, player_position{ 0 }, unique_index{0} {
        for (auto & bp: box_positions) { bp = 0; }
    }
    unsigned long long hash() const;
};

    inline bool operator==(const BoxState & l, const BoxState & r) {
        return l.player_position == r.player_position
            && l.box_positions   == r.box_positions;
    }
}

namespace std {
template <> struct hash<Sokoban::BoxState> {
    typedef Sokoban::BoxState argument_type;
    typedef unsigned long long result_type;

    result_type operator()(argument_type const & bs) const noexcept {
        return bs.hash();
    }
};
}

#endif
