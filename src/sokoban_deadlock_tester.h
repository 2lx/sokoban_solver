#ifndef SOKOBAN_DEADLOCK_TESTER_H
#define SOKOBAN_DEADLOCK_TESTER_H

#include "sokoban_common.h"
#include <vector>
#include <functional>

namespace Sokoban
{
class BoardState;

class DeadlockTester {
    std::vector<std::vector<std::function<bool()>>> _checks;
    size_t _width, _height;

    bool test_landscape(const BoardState & state,
            const std::vector<index_t> & walls,
            const std::vector<index_t> & spaces,
            const std::vector<index_t> & goals) const;

public:
    DeadlockTester() = default;

    bool initialize(const BoardState & state);
    bool test_for_index(index_t ind) const;
};
}

#endif
