#include "sokoban_deadlock_tester.h"
#include "sokoban_board_state.h"

using namespace Sokoban;
using namespace std;

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
    auto check1 = [&state](const index_t i1) {
        return state.is_box(i1);
    };

    auto check3 = [&state](const index_t i1, const index_t i2, const index_t i3) {
        return state.is_box(i1) && state.is_box(i2) && state.is_box(i3);
    };

    array<pair<bool, bool>, 4> reflections = {{
        {false, false}, {true, false}, {false, true}, {true, true}
    }};

    for (index_t ind = 0; ind < state.tile_count(); ind++) {
        if (state.is_wall(ind)) { continue; }

        /* bool vrefl = false, hrefl = false; */
        for (const auto [vrefl, hrefl]: reflections) {
            const index_t ind_ul = symmetric_index(ind, -1, -1, vrefl, hrefl);
            const index_t ind_u  = symmetric_index(ind, -1,  0, vrefl, hrefl);
            /* const index_t ind_ur = symmetric_index(i, -1,  1, vrefl, hrefl); */
            const index_t ind_l  = symmetric_index(ind,  0, -1, vrefl, hrefl);
            /* const index_t ind_r  = symmetric_index(i, -1,  1, vrefl, hrefl); */
            /* const index_t ind_dl = symmetric_index(i, -1, -1, vrefl, hrefl); */
            /* const index_t ind_d  = symmetric_index(i, -1,  0, vrefl, hrefl); */
            /* const index_t ind_dr = symmetric_index(i, -1,  1, vrefl, hrefl); */

            if (state.box_count() < 2) { continue; }
            // check if there are two walls, and there is a box on the left
            // ##
            // $?
            if (    state.is_wall(ind_ul) && state.is_wall(ind_u)
                && !state.is_wall(ind_l)
                && (!state.is_goal(ind) || !state.is_goal(ind_l))) {
                _checks[ind].push_back(bind(check1, ind_l));
            }
            // #$
            // #?
            if (    state.is_wall(ind_ul) && state.is_wall(ind_l)
                && !state.is_wall(ind_u)
                && (!state.is_goal(ind) || !state.is_goal(ind_u))) {
                _checks[ind].push_back(bind(check1, ind_u));
            }

            if (state.box_count() < 4) { continue; }
            // check if box is in square with no walls, and there are three other boxes,
            // and there is at least one of them is not on a goal
            // $$
            // $?
            if (    !state.is_wall(ind_ul) && !state.is_wall(ind_u) && !state.is_wall(ind_l)
                && (   !state.is_goal(ind_ul) || !state.is_goal(ind_u)
                    || !state.is_goal(ind_l)  || !state.is_goal(ind))) {
                _checks[ind].push_back(bind(check3, ind_ul, ind_u, ind_l));
            }

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
