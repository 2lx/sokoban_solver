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
        box_states.max_load_factor(100.);
        box_states.reserve(760007);
    }

    std::pair<bool, unsigned> insert_state(const BoxState & bstate) {
        BoxState newstate{ bstate };
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

    /* bool contains(const BoxState bs) const { */
    /*     return false; */
    /* } */

    /* BoxState get_state(const unsigned id) const { */
    /*     return *std::find_if(begin(box_states), end(box_states), */
    /*                          [id](auto bs) { return bs.unique_index == id; }); */
    /* } */
};

}

#endif
