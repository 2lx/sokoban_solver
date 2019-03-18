#ifndef SOKOBAN_TRANSPOSITION_TABLE_H
#define SOKOBAN_TRANSPOSITION_TABLE_H

#include <unordered_set>
#include <utility>
#include <algorithm>

#include "sokoban_boxstate.h"

namespace Sokoban
{

class TranspositionTable {
    std::unordered_set<BoxState, std::hash<BoxState>> box_states;
    unsigned count;

public:
    TranspositionTable() : box_states{}, count{ 0u } {
        box_states.max_load_factor(10.);
        box_states.reserve(19007);
    }

    std::pair<bool, unsigned> insert_state(BoxState newstate) {
        newstate.unique_index = count;
        auto [it, inserted] = box_states.insert(newstate);
        if (inserted) { count++; }

        return std::make_pair(inserted, it->unique_index);
    }

    void print() const {
        for (const auto bs: box_states) {
            std::cout << "state " << bs.unique_index << ": "
                      << "[player={" << bs.player_position << "}, boxes={";
            for (const auto ind: bs.box_positions) {
                std::cout << ind << ' ';
            }
            std::cout << "}]" << std::endl;
        }
    }
};

}

#endif
