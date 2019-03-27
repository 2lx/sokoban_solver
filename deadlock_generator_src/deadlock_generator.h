#ifndef DEADLOCK_GENERATOR_H
#define DEADLOCK_GENERATOR_H

#include "deadlock_info.h"
#include <string>
#include <vector>

namespace Sokoban
{
class DeadlockGenerator {
    std::string level_pattern;
    size_t goalfi;
    size_t boxfi;
    size_t testi;
    size_t width;

    bool check_solution(const std::string & level);

public:
    DeadlockGenerator() = default;

    void initialize(const std::string & level);
    std::vector<Sokoban::DeadlockInfo> generate();
};

}

#endif
