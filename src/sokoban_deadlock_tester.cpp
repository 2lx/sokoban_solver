#include "sokoban_deadlock_tester.h"
#include "sokoban_board_state.h"
#include "deadlocks.h"

#include <cassert>
#include <iostream>

using namespace Sokoban;
using namespace std;

optional<index_t> DeadlockTester::symmetric_index(size_t ind, const pair<int, int> & diff,
                                        const pair<bool, bool> & refl) const {
    assert(ind < _width * _height);
    const int space_to_top    = static_cast<int>(ind / _width);
    const int space_to_left   = static_cast<int>(ind % _width);
    const int space_to_right  = static_cast<int>(_width - space_to_left - 1);
    const int space_to_bottom = static_cast<int>(_height - space_to_top - 1);

    if (diff.first  < 0 && -diff.first  > space_to_top)    return nullopt;
    if (diff.first  > 0 &&  diff.first  > space_to_bottom) return nullopt;
    if (diff.second < 0 && -diff.second > space_to_left)   return nullopt;
    if (diff.second < 0 &&  diff.second > space_to_right)  return nullopt;

    const int v = (refl.first  ? -static_cast<int>(_width) : static_cast<int>(_width));
    const int h = (refl.second ? -1 : 1);

    return static_cast<index_t>(ind + v*diff.first + h*diff.second);
}

bool DeadlockTester::test_landscape(const BoardState & state,
                const DeadlockInfo & dlinfo, const pair<bool, bool> & refl, index_t ind) const
{
    if (state.is_wall(ind)) return false;

    auto get_index = [&](const Point & p){
        return symmetric_index(ind, { p.y, p.x }, refl);
    };

    const auto & [wallpoints, spacepoints, boxpoints, goalpoints, indep] = dlinfo;

    for (const auto & wp: wallpoints) {
        const auto wi = get_index(wp);
        if (!wi.has_value())    { return false; }
        if (!state.is_wall(wi.value())) { return false; }
    }

    for (const auto & sp: spacepoints) {
        const auto si = get_index(sp);
        if (!si.has_value())   { return false; }
        if ( state.is_wall(si.value())) { return false; }
    }

    for (const auto & gione: goalpoints) {
        bool all_of_indexes_is_goals = true;

        for (const auto gp: gione) {
            const auto gi = get_index(gp);
            if (!gi.has_value()) return false;
            all_of_indexes_is_goals &= state.is_goal(gi.value());
        }

        if (all_of_indexes_is_goals) { return false; }
    }
    return true;
}

bool DeadlockTester::initialize(const BoardState & state) {
    _width  = state.width();
    _height = state.height();
    _checks.resize(state.tile_count());

    // captures the reference to a member of another class!
    auto check_all = [&state](const vector<index_t> & inds) {
        return all_of(begin(inds), end(inds),
            [&state](index_t bi){ return state.is_box(bi); });
    };

    array<pair<bool, bool>, 4> reflections = {{
        {false, false}, {true, false}, {false, true}, {true, true}
    }};

    for (index_t ind = 0; ind < state.tile_count(); ind++) {
        if (state.is_wall(ind)) { continue; }

        for (const auto & refl: reflections) {
            for (const auto & dlset: generated_deadlocks) {
                for (const auto & dlinfo: dlset) {
                    if (test_landscape(state, dlinfo, refl, ind)) {
                        vector<index_t> boxinds;

                        transform(begin(dlinfo.boxes), end(dlinfo.boxes),
                                  back_inserter(boxinds), [&](auto p) {
                                        const auto symi = symmetric_index(ind, { p.y, p.x }, refl);
                                        assert(symi.has_value());
                                        return symi.value();
                                  });

                        _checks[ind].push_back(bind(check_all, boxinds));
                    }
                }
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
