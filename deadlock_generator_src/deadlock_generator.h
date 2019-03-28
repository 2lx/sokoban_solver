#ifndef DEADLOCK_GENERATOR_H
#define DEADLOCK_GENERATOR_H

#include "deadlock_info.h"
#include <string>
#include <vector>

namespace Sokoban
{
class DeadlockGenerator {
    struct Combination {
        std::vector<size_t> walls;
        std::vector<size_t> boxes;
        std::vector<size_t> goals;
        std::vector<size_t> extra_boxes;
        std::vector<size_t> extra_goals;

        size_t box_count()  const { return boxes.size() + extra_boxes.size(); }
        size_t goal_count() const { return goals.size() + extra_goals.size(); }
    };

    std::string _level_pattern;
    std::vector<size_t> _extra_goal_indexes;
    std::vector<size_t> _extra_box_indexes;
    size_t _test_index;
    size_t _width;

    std::vector<size_t> _wall_all_indexes;
    std::vector<size_t> _box_all_indexes;
    std::vector<size_t> _goal_all_indexes;
    std::vector<DeadlockInfo> _result;

    bool check_solution(const std::string & level);
    void combine_boxes(Combination & ci);
    void combine_goals(Combination & ci);

    void insert_deadlock(const Combination & ci, bool not_depends_on_goals);

public:
    DeadlockGenerator() = default;

    void initialize(const std::string & level);
    std::vector<Sokoban::DeadlockInfo> generate();
};

}

#endif
