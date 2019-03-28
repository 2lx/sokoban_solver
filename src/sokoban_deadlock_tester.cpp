#include "sokoban_deadlock_tester.h"
#include "sokoban_board_state.h"
#include "deadlocks.h"

#include <cassert>
#include <iostream>

using namespace Sokoban;
using namespace std;


bool DeadlockTester::test_landscape(const BoardState & state, const vector<index_t> & walls,
        const vector<index_t> & spaces, const vector<index_t> & goals) const {
    for (auto wi: walls) {
        if (!state.is_wall(wi)) { return false; }
    }

    for (auto si: spaces) {
        if (state.is_wall(si)) { return false; }
    }

    bool all_goals = all_of(begin(goals), end(goals),
            [&state](index_t gi){ return state.is_goal(gi); });

    return !all_goals;
}

bool DeadlockTester::initialize(const BoardState & state) {
    _width  = state.width();
    _height = state.height();
    _checks.resize(state.tile_count());

    auto symmetric_index =
                [width = static_cast<int>(_width)]
                (size_t ind, int vdiff, int hdiff, bool vreflect, bool hreflect) {
        const int v = (vreflect ? -width : width);
        const int h = (hreflect ? -1 : 1);

        return static_cast<index_t>(ind + v*vdiff + h*hdiff);
    };

    // captures the reference to a member of another class!
    auto check_all = [&state](const vector<index_t> & inds) {
        return all_of(begin(inds), end(inds),
            [&state](index_t bi){ return state.is_box(bi); });
    };

    /* auto check1 = [&state](const index_t i1) { */
    /*     return state.is_box(i1); */
    /* }; */
    /*  */
    /* auto check3 = [&state](const index_t i1, const index_t i2, const index_t i3) { */
    /*     return state.is_box(i1) && state.is_box(i2) && state.is_box(i3); */
    /* }; */

    array<pair<bool, bool>, 4> reflections = {{
        {false, false}, {true, false}, {false, true}, {true, true}
    }};

    const vector<vector<DeadlockInfo>> all_deadlocks = { deadlocks1x2, deadlocks2x2 };

    for (index_t ind = 0; ind < state.tile_count(); ind++) {
        if (state.is_wall(ind)) { continue; }

        for (const auto [vrefl, hrefl]: reflections) {
            auto get_index = [&](const Point & p){
                return symmetric_index(ind, p.y, p.x, vrefl, hrefl);
            };

            for (const auto & deadlocks: all_deadlocks) {
                for (auto [wallpoints, spacepoints, boxpoints, goalpoints, indep]: deadlocks) {
                    if (!indep) { assert(true); } // isn't yet implemented

                    vector<index_t> wallinds;
                    transform(begin(wallpoints), end(wallpoints),
                            back_inserter(wallinds), get_index);

                    vector<index_t> spaceinds;
                    transform(begin(spacepoints), end(spacepoints),
                            back_inserter(spaceinds), get_index);

                    vector<index_t> boxinds;
                    transform(begin(boxpoints),  end(boxpoints),
                            back_inserter(boxinds),  get_index);

                    vector<index_t> goalinds;
                    transform(begin(goalpoints), end(goalpoints),
                            back_inserter(goalinds), get_index);

                    if (test_landscape(state, wallinds, spaceinds, goalinds)) {
                        _checks[ind].push_back(bind(check_all, boxinds));
                    }
                }
            }

            /* const index_t ind_ul = symmetric_index(ind, -1, -1, vrefl, hrefl); */
            /* const index_t ind_u  = symmetric_index(ind, -1,  0, vrefl, hrefl); */
            /* const index_t ind_l  = symmetric_index(ind,  0, -1, vrefl, hrefl); */
            /*  */
            /* if (state.box_count() < 2) { continue; } */
            /* // check if there are two walls, and there is a box on the left */
            /* // ## */
            /* // $? */
            /* if (    state.is_wall(ind_ul) && state.is_wall(ind_u) */
            /*     && !state.is_wall(ind_l) */
            /*     && (!state.is_goal(ind) || !state.is_goal(ind_l))) { */
            /*     _checks[ind].push_back(bind(check1, ind_l)); */
            /* } */
            /* // #$ */
            /* // #? */
            /* if (    state.is_wall(ind_ul) && state.is_wall(ind_l) */
            /*     && !state.is_wall(ind_u) */
            /*     && (!state.is_goal(ind) || !state.is_goal(ind_u))) { */
            /*     _checks[ind].push_back(bind(check1, ind_u)); */
            /* } */
            /*  */
            /* if (state.box_count() < 4) { continue; } */
            /* // check if box is in square with no walls, and there are three other boxes, */
            /* // and there is at least one of them is not on a goal */
            /* // $$ */
            /* // $? */
            /* if (    !state.is_wall(ind_ul) && !state.is_wall(ind_u) && !state.is_wall(ind_l) */
            /*     && (   !state.is_goal(ind_ul) || !state.is_goal(ind_u) */
            /*         || !state.is_goal(ind_l)  || !state.is_goal(ind))) { */
            /*     _checks[ind].push_back(bind(check3, ind_ul, ind_u, ind_l)); */
            /* } */
        }
    }
    return true;
}

bool DeadlockTester::test_for_index(index_t ind) const {
    for (auto & fn: _checks[ind]) {
        if (fn()) { return true; }
    }
    return false;
}
