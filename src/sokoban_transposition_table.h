#ifndef SOKOBAN_TRANSPOSITION_TABLE_H
#define SOKOBAN_TRANSPOSITION_TABLE_H

#include <unordered_set>
#include <utility>
#include <algorithm>

#include "sokoban_boxstate.h"

namespace Sokoban
{

class TranspositionTable {
    std::unordered_set<BoxState, std::hash<BoxState>> _box_states;
    unsigned count;

public:
    TranspositionTable() : _box_states{}, count{ 0u } {
        _box_states.max_load_factor(10.);
        _box_states.reserve(80833);
    }

    size_t size() const { return _box_states.size(); }

    std::pair<bool, unsigned> insert_state(BoxState newstate) {
        newstate.unique_index = count;
        auto [it, inserted] = _box_states.insert(newstate);
        if (inserted) { count++; }

        return std::make_pair(inserted, it->unique_index);
    }

    BoxState find(const stateid_t unique_id) const {
        for (size_t i = 0u; i < _box_states.bucket_count(); ++i) {
            for (auto it = _box_states.cbegin(i); it != _box_states.cend(i); ++it) {
                if (it->unique_index == unique_id) { return *it; }
            }
        }
    }

    void print() const {
        size_t max = 0;
        for (size_t i = 0; i < _box_states.bucket_count(); i++) {
            if (_box_states.bucket_size(i) > max) {
                max = _box_states.bucket_size(i);
            }
            std::cout << _box_states.bucket_size(i) << ' ';
        }
        std::cout << std::endl;

        /* for (const auto bs: _box_states) { */
        /*     std::cout << "state " << bs.unique_index << ": " */
        /*               << "[player={" << bs.player_position << "}, boxes={"; */
        /*     for (const auto ind: bs.box_positions) { */
        /*         std::cout << ind << ' '; */
        /*     } */
        /*     std::cout << "}]" << std::endl; */
        /* } */
    }
};

}

#endif
