#include "deadlock_generator.h"
#include "sokoban_solver.h"

using namespace std;
using namespace Sokoban;

void DeadlockGenerator::initialize(const string & level) {
    level_pattern = level;

    goalfi = distance(begin(level_pattern),
                                         find(begin(level_pattern), end(level_pattern), '1'));
    boxfi = distance(begin(level_pattern),
                                        find(begin(level_pattern), end(level_pattern), '2'));
    testi = distance(begin(level_pattern),
                                        find(begin(level_pattern), end(level_pattern), '$'));
    width = distance(begin(level_pattern),
                                        find(begin(level_pattern), end(level_pattern), '\n')) + 1;

    level_pattern[goalfi] = ' ';
    level_pattern[boxfi] = ' ';
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
    vector<DeadlockInfo> result;

    vector<size_t> wall_all_indexes;
    auto it = find(begin(level_pattern), end(level_pattern), '?');
    while (it != end(level_pattern)) {
        wall_all_indexes.push_back(distance(begin(level_pattern), it));
        it = find(next(it), end(level_pattern), '?');
    }

    const size_t bit_count = wall_all_indexes.size();

    for (size_t wallbits = 0; wallbits < (1u << bit_count); wallbits++) {
        vector<size_t> box_all_indexes;
        vector<size_t> wall_indexes;
        vector<size_t> goal_all_indexes;

        for (unsigned i = 0; i < bit_count; ++i) {
            if (wallbits & (1u << i)) {
                wall_indexes.push_back(wall_all_indexes[i]);
                level_pattern[wall_all_indexes[i]] = '#';
            } else {
                level_pattern[wall_all_indexes[i]] = ' ';
                box_all_indexes.push_back(wall_all_indexes[i]);
                goal_all_indexes.push_back(wall_all_indexes[i]);
             }
        }
        goal_all_indexes.push_back(testi);

        for (size_t boxbits = 0; boxbits < (1u << box_all_indexes.size()); boxbits++) {
            vector<size_t> box_indexes;
            for (unsigned i = 0; i < box_all_indexes.size(); ++i) {
                if (boxbits & (1u << i)) {
                    box_indexes.push_back(box_all_indexes[i]);
                    level_pattern[box_all_indexes[i]] = '$';
                } else {
                    level_pattern[box_all_indexes[i]] = ' ';
                }
            }

            // goals permutation
            // if there are all possible goal indexes are goals - there is always solution
            size_t goal_permutation_max = (1u << (goal_all_indexes.size())) - 1;
            bool is_always_no_solution = true;
            bool has_result = false;

            for (size_t goalbits = 0; goalbits < goal_permutation_max; goalbits++) {
                vector<size_t> goal_indexes;
                for (unsigned i = 0; i < goal_all_indexes.size(); ++i) {
                    if (goalbits & (1u << i)) {
                        goal_indexes.push_back(goal_all_indexes[i]);

                        if (level_pattern[goal_all_indexes[i]] == '$') {
                            level_pattern[goal_all_indexes[i]] = '*';
                        } else {
                            level_pattern[goal_all_indexes[i]] = '.';
                        }
                    }
                }

                vector<size_t> goal_add_indexes;
                vector<size_t> box_add_indexes;

                while ( goal_indexes.size() + goal_add_indexes.size()
                      < box_indexes.size()  + box_add_indexes.size() + 1)
                {
                    const size_t newi = goalfi + goal_add_indexes.size();

                    goal_add_indexes.push_back(newi);
                    level_pattern[newi] = '.';
                }

                while ( goal_indexes.size() + goal_add_indexes.size()
                      > box_indexes.size()  + box_add_indexes.size() + 1)
                {
                    const size_t newi = boxfi + box_add_indexes.size();

                    box_add_indexes.push_back(newi);
                    level_pattern[newi] = '$';
                }

                /* cout << level_pattern; */
                /* cout << "walls:" << setw(10) << string_join(wall_indexes) << "  " */
                /*      << "boxes:" << setw(10) << string_join(box_indexes)  << "  " */
                /*      << "goals:" << setw(10) << string_join(goal_indexes) << setw(5); */
                /*  */
                /* if (check_solution(level_pattern)) { cout << "YES"; } */
                /* else { cout << "NO"; } */
                /* cout << '\n' << endl; */

                if (!check_solution(level_pattern)) {
                    /* cout << level_pattern; */
                    /* cout << "walls:" << setw(10) << string_join(wall_indexes) << "  " */
                    /*     << "boxes:" << setw(10) << string_join(box_indexes)  << "  " */
                    /*     << "goals:" << setw(10) << string_join(goal_indexes) << setw(5); */
                    /* cout << '\n' << endl; */


                    auto getc = [testi=testi, width=width](size_t ind) {
                        return offset(testi, ind, width);
                    };

                    DeadlockInfo dli;
                    transform(begin(wall_indexes), end(wall_indexes), back_inserter(dli.walls), getc);
                    transform(begin(box_indexes),  end(box_indexes),  back_inserter(dli.boxes), getc);
                    transform(begin(goal_indexes), end(goal_indexes), back_inserter(dli.goals), getc);
                    dli.independent_of_goals = is_always_no_solution;

                    result.push_back(dli);
                    has_result = true;
                } else {
                    is_always_no_solution = false;
                }

                for (auto i: goal_add_indexes) { level_pattern[i] = ' '; }
                for (auto i:  box_add_indexes) { level_pattern[i] = ' '; }

                for (unsigned i = 0; i < goal_indexes.size(); ++i) {
                    if (level_pattern[goal_indexes[i]] == '*') {
                        level_pattern[goal_indexes[i]] = '$';
                    } else {
                        level_pattern[goal_indexes[i]] = ' ';
                    }
                }
            }
            // test for independent, remove extra solutions
            if (is_always_no_solution) {
                while (result.size() > 1
                    && result[result.size() - 1].walls == result[result.size() - 2].walls
                    && result[result.size() - 1].boxes == result[result.size() - 2].boxes) {
                    result.pop_back();
                }
                result.back().goals.clear();
            } else if (has_result) {
                size_t ind = 0;
                if (!result.empty()) { result.back().independent_of_goals = false; }
                while (result.size() - ind > 1
                    && result[result.size() - 1 - ind].walls == result[result.size() - 2 - ind].walls
                    && result[result.size() - 1 - ind].boxes == result[result.size() - 2 - ind].boxes) {
                    result[result.size() - 2 - ind].independent_of_goals = false;
                    ind++;
                }
            }

            for (unsigned i = 0; i < box_all_indexes.size(); ++i) {
                level_pattern[box_all_indexes[i]] = ' ';
            }
        }
        for (unsigned i = 0; i < wall_all_indexes.size(); ++i) {
            level_pattern[wall_all_indexes[i]] = ' ';
        }
    }

    return result;
}
