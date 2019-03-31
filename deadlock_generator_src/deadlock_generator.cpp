#include "deadlock_generator.h"
#include "sokoban_solver.h"
#include "string_join.h"
#include <set>

using namespace std;
using namespace Sokoban;

void DeadlockGenerator::initialize(const string & level) {
    _level_pattern = level;
    _width  = distance(begin(_level_pattern),
            find(begin(_level_pattern), end(_level_pattern), '\n')) + 1;

    for (auto it = begin(level); it != end(level); ++it) {
        const size_t index = distance(begin(level), it);
        if (*it == '1') { _extra_goal_indexes.push_back(index); }
        if (*it == '2') {  _extra_box_indexes.push_back(index); }
        if (*it == '?') {   _wall_all_indexes.push_back(index); }
        if (*it == '$') { _test_index = index; }
    }

    for (auto index: _extra_goal_indexes) { _level_pattern[index] = ' '; }
    for (auto index: _extra_box_indexes)  { _level_pattern[index] = ' '; }
}

bool DeadlockGenerator::check_solution(const string & level) {
    istringstream iss(level);

    Sokoban::Solver solver;
    if (!solver.read_level_data(iss)) {
        cout << "ERROR LEVEL FORMAT" << endl; throw;
    }

    return solver.solve();
}

std::vector<DeadlockInfo> DeadlockGenerator::generate() {
    const size_t bit_count = _wall_all_indexes.size();

    const string level_bkp = _level_pattern;
    for (size_t wallbits = 0; wallbits < (1u << bit_count); wallbits++) {
        _level_pattern = level_bkp;
        _box_all_indexes.clear();
        _goal_all_indexes.clear();
        Combination ci;

        for (unsigned i = 0; i < bit_count; ++i) {
            if (wallbits & (1u << i)) {
                ci.walls.push_back(_wall_all_indexes[i]);
                _level_pattern[_wall_all_indexes[i]] = '#';
            } else {
                _level_pattern[_wall_all_indexes[i]] = ' ';
                _box_all_indexes.push_back(_wall_all_indexes[i]);
                _goal_all_indexes.push_back(_wall_all_indexes[i]);
             }
        }
        _goal_all_indexes.push_back(_test_index);

        combine_boxes(ci);
    }

    return _result;
}

void DeadlockGenerator::combine_boxes(Combination & ci) {
    const string level_bkp = _level_pattern;
    for (size_t boxbits = 0; boxbits < (1u << _box_all_indexes.size()); boxbits++) {
        _level_pattern = level_bkp;
        ci.boxes.clear();

        for (unsigned i = 0; i < _box_all_indexes.size(); ++i) {
            if (boxbits & (1u << i)) {
                ci.boxes.push_back(_box_all_indexes[i]);
                _level_pattern[_box_all_indexes[i]] = '$';
            } else {
                _level_pattern[_box_all_indexes[i]] = ' ';
            }
        }

        combine_goals(ci);
    }
}

// iterates through combinations of goals
void DeadlockGenerator::combine_goals(Combination & ci) {
    bool no_solutions_at_all = true;
    vector<pair<Combination, bool>> local_results;

    // if there are all possible goal indexes are goals - there is always solution
    // so we do not need to check this combination ('goalbits' will not reach '11..11')
    size_t goal_combination_count = (1u << (_goal_all_indexes.size())) - 1;
    const string level_bkp = _level_pattern;
    for (size_t goalbits = 0; goalbits < goal_combination_count; goalbits++) {
        _level_pattern = level_bkp;
        ci.goals.clear();
        ci.extra_boxes.clear();
        ci.extra_goals.clear();

        for (unsigned i = 0; i < _goal_all_indexes.size(); ++i) {
            if (goalbits & (1u << i)) {
                ci.goals.push_back(_goal_all_indexes[i]);

                if (_level_pattern[_goal_all_indexes[i]] == '$') {
                    _level_pattern[_goal_all_indexes[i]] = '*';
                } else {
                    _level_pattern[_goal_all_indexes[i]] = '.';
                }
            }
        }

        while (ci.goal_count() < ci.box_count() + 1) {
            const size_t exgoali = _extra_goal_indexes[ci.extra_goals.size()];

            ci.extra_goals.push_back(exgoali);
            _level_pattern[exgoali] = '.';
        }

        while (ci.goal_count() > ci.box_count() + 1) {
            const size_t exboxi = _extra_box_indexes[ci.extra_boxes.size()];

            ci.extra_boxes.push_back(exboxi);
            _level_pattern[exboxi] = '$';
        }

        /* cout << _level_pattern; */
        /* cout << "walls:" << setw(10) << string_join(ci.walls) << "  " */
        /*      << "boxes:" << setw(10) << string_join(ci.boxes) << "  " */
        /*      << "goals:" << setw(10) << string_join(ci.goals) << setw(5); */

        /* if (check_solution(_level_pattern)) { cout << "YES"; } */
        /* else { cout << "NO"; } */
        /* cout << '\n' << endl; */

        if (!check_solution(_level_pattern)) {
            local_results.push_back({ ci, no_solutions_at_all });
        } else {
            no_solutions_at_all = false;
        }
    }

    // test dependency of goals, remove extra solutions
    if (!local_results.empty()) {
        if (no_solutions_at_all) {
            local_results.front().first.goals.clear();
            insert_deadlock(local_results.front().first, true);
        } else {
            for (const auto & [combination, ingore]: local_results) {
                insert_deadlock(combination, false);
            }
        }
    }

}

void DeadlockGenerator::insert_deadlock(const Combination & ci, bool not_depends_on_goals) {
    auto getc = [testi=_test_index, _width=_width](size_t ind) {
        return offset(testi, ind, _width);
    };

    set<size_t> spaces(begin(_wall_all_indexes), end(_wall_all_indexes));
    for (const auto wi: ci.walls) { spaces.erase(wi); }

    DeadlockInfo dli;
    transform(begin(ci.walls), end(ci.walls), back_inserter(dli.walls),  getc);
    transform(begin(spaces),   end(spaces),   back_inserter(dli.spaces), getc);
    transform(begin(ci.boxes), end(ci.boxes), back_inserter(dli.boxes),  getc);

    if (not_depends_on_goals) {
        dli.goalsets.push_back({});
        transform(begin(_goal_all_indexes), end(_goal_all_indexes),
                  back_inserter(dli.goalsets.front()), getc);
    } else {
        dli.goalsets.push_back({});
        transform(begin(ci.goals), end(ci.goals),
                  back_inserter(dli.goalsets.front()), getc);
    }
    dli.independent_of_goals = not_depends_on_goals;

    _result.push_back(dli);
}
