#ifndef SOKOBAN_DEADLOCK_TESTER_H
#define SOKOBAN_DEADLOCK_TESTER_H

#include "sokoban_common.h"

#include <vector>
#include <functional>

namespace Sokoban
{
class BoardState;
class DeadlockInfo;

class DeadlockTester {
    std::vector<std::vector<std::function<bool()>>> _checks;
    size_t _width, _height;

    std::optional<index_t> symmetric_index(size_t ind,
                        const std::pair<int, int> & diff,
                        const std::pair<bool, bool> & refl) const;
    bool test_landscape(const BoardState & state, const DeadlockInfo & dlinfo,
                        const std::pair<bool, bool> & refl, index_t ind) const;

public:
    DeadlockTester() = default;

    bool initialize(const BoardState & state);
    bool test_for_index(index_t ind) const;
};
}

#endif
