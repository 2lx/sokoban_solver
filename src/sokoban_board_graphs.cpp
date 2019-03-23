#include "sokoban_board_graphs.h"
#include "sokoban_board_state.h"

#include <set>
#include <iostream>
#include "string_join.h"

using namespace Sokoban;
using namespace std;

bool BoardGraphs::initialize(const BoardState & state) {
    _count     = state.tile_count();
    _box_count = state.box_count();
    _all_moves.resize(_count);

    DGraph reverse_pushes;
    reverse_pushes.resize(_count);

    for (index_t i = 0; i < _count; i++) {
        if (state.is_wall(i)) { continue; }

        const index_t ind_u = static_cast<index_t>(i - state.width());
        const index_t ind_l = static_cast<index_t>(i - 1u);
        const index_t ind_r = static_cast<index_t>(i + 1u);
        const index_t ind_d = static_cast<index_t>(i + state.width());

        const bool is_topmost    = i / state.width() == 0;
        const bool is_leftmost   = i % state.width() == 0;
        const bool is_rightmost  = i % state.width() == state.width()  - 1;
        const bool is_bottommost = i / state.width() == state.height() - 1;

        const bool is_passable_u = !is_topmost    && !state.is_wall(ind_u);
        const bool is_passable_l = !is_leftmost   && !state.is_wall(ind_l);
        const bool is_passable_r = !is_rightmost  && !state.is_wall(ind_r);
        const bool is_passable_d = !is_bottommost && !state.is_wall(ind_d);

        // insert reversed edges
        if (is_passable_u && is_passable_d) {
            reverse_pushes.insert_edge(ind_u, i);
            reverse_pushes.insert_edge(ind_d, i);
        }
        if (is_passable_l && is_passable_r) {
            reverse_pushes.insert_edge(ind_l, i);
            reverse_pushes.insert_edge(ind_r, i);
        }

        if (is_passable_u) { _all_moves.insert_edge(i, ind_u); }
        if (is_passable_l) { _all_moves.insert_edge(i, ind_l); }
        if (is_passable_r) { _all_moves.insert_edge(i, ind_r); }
        if (is_passable_d) { _all_moves.insert_edge(i, ind_d); }
    }

    /* reverse_pushes.print(); */
    /* cout << "REVERSE PUSHES:\n" << endl; */
    /* auto nodes = reverse_pushes.nodes(); */
    /* vector<index_t> pushes; */
    /* for_each (nodes.begin(state.goal_index(5)), nodes.end(), */
    /*         [&pushes](auto ind){ pushes.push_back(ind); }); */
    /* cout << string_join(pushes, ", ") << endl; */
    /* state.print(pushes); */

    bipartite_matching(state, reverse_pushes);
    calculate_goals_distances(state, reverse_pushes);
    calculate_goals_order(state, reverse_pushes);
    calculate_routes(reverse_pushes);
    narrow_moves(state.box_indexes());

    return true;
}

void BoardGraphs::bipartite_matching(const BoardState & state, const DGraph & reverse_pushes) {
    // collect all achievable goals for each box
    _boxes_goals.resize(_box_count, {});
    for (const auto goali: state.goal_indexes()) {
        const auto passable_boxes = reverse_pushes.check_if_passable(goali, state.box_indexes());

        for (size_t i = 0; i < passable_boxes.size(); ++i) {
            if (passable_boxes[i]) {
                _boxes_goals[i].push_back(goali);
            }
        }
    }

    // perform the bipartite matching
    set<index_t> matched_goals;
    auto it = find_if(begin(_boxes_goals), end(_boxes_goals),
                      [](auto bg){ return bg.size() == 1; });
    while (it != end(_boxes_goals)) {
        index_t goali = (*it)[0];
        auto erase_goali = [goali](auto & vec) {
            vec.erase(remove(begin(vec), end(vec), goali), end(vec));
        };

        for_each(begin(_boxes_goals), it, erase_goali);
        for_each(next(it), end(_boxes_goals), erase_goali);

        matched_goals.insert(goali);
        it = find_if(begin(_boxes_goals), end(_boxes_goals),
                    [&matched_goals](auto bg){ return bg.size() == 1
                                                   && matched_goals.count(bg[0]) == 0; });
    }
}

void BoardGraphs::calculate_goals_distances(const BoardState & state,
                                            const DGraph & reverse_pushes) {
    _goals_distances.resize(_box_count);
    auto nodes = reverse_pushes.nodes_with_distances();

    for (size_t i = 0; i < _box_count; ++i) {
        std::for_each(nodes.begin(state.goal_index(i)), nodes.end(), [](auto){});
        _goals_distances[i] = nodes.distances();
    }
}

void BoardGraphs::calculate_goals_order(const BoardState & state,
                                        const DGraph & reverse_pushes) {
    vector<pair<size_t, index_t>> unordered_goals;
    for (size_t i = 0; i < _box_count; ++i) {
        unordered_goals.push_back({ i, state.goal_index(i) });
    };
    DGraph base_rpushes{ reverse_pushes };

    while (!unordered_goals.empty()) {
        for (const auto [gi, goali]: unordered_goals) {
            DGraph trpushes{ base_rpushes };
            trpushes.remove_node(goali);

            /* bool at_least_one_passable = false; */
            bool all_passable = true;

            // for each remaining goal, check if there are any boxes,
            // that have become unpassable for it (after removing the node)
            for (const auto [ttt, chgi]: unordered_goals) {
                if (chgi == goali) { continue; }

                const auto passbits = trpushes.check_if_passable(chgi, state.box_indexes());
                /* cout << string_join(passbits, "") << endl; */

                for (size_t i = 0; i < _box_count; ++i) {
                    if (binary_search(begin(_boxes_goals[i]), end(_boxes_goals[i]), chgi)) {
                        if (!passbits[i]) { all_passable = false; }
                    }
                }
            }

            if (all_passable) {
                _goals_order.push_back(gi);
                unordered_goals.erase(remove(begin(unordered_goals), end(unordered_goals),
                                             make_pair(gi, goali)),
                                      end(unordered_goals));
                base_rpushes.remove_node(goali);
                break;
            }
        }
    }
}

// calculates the routes of the boxes (all possible pushes for every box)
// The routes depend on bipartite matching results and results of minimum matching algorithm
void BoardGraphs::calculate_routes(const DGraph & reverse_pushes) {
    _boxes_routes.resize(_box_count, reverse_pushes);

    for (size_t i = 0; i < _box_count; ++i) {
        _boxes_routes[i].remove_impassable(_boxes_goals[i]);
        _boxes_routes[i].transpose();
    }
}

size_t BoardGraphs::ordered_boxes_on_goals(const BoardState & state) const {
    size_t result = 0u;
    for (const auto i: _goals_order) {
        index_t goali = state.goal_index(i);
        if (state.is_box(goali)) { result++; continue; }
        else { break; }
    }
    return result;
}

pair<size_t, size_t> BoardGraphs::push_distances(const BoardState & state,
                                                 size_t boxi, const PushInfo & pi) const {
    for (const auto i: _goals_order) {
        index_t goali = state.goal_index(i);

        // if there is already a box on the goal
        if (state.is_box(goali)) { continue; }

        // if box can't move to that goal
        if (!binary_search(begin(_boxes_goals[boxi]), end(_boxes_goals[boxi]), goali)) { continue; }

        // we found the goal with the highest priority for considered box
        // return distances before and after the pushing
        const size_t dist_from = _goals_distances[i][pi.from()];
        const size_t dist_to   = _goals_distances[i][pi.to()];
        return make_pair(dist_from, dist_to);
    }

    assert(true);
    return {};
}

index_t BoardGraphs::min_move_index(index_t player) const {
    auto nodes = _boxdep_moves.nodes();

    // we don't use min_element because the graph iterator has the input_iterator category,
    // so we can't read a value twice
    index_t min = MAX_TILE_COUNT;
    for_each (nodes.begin(player), nodes.end(),
              [&min](const auto ind){ if (min > ind) { min = ind; } });

    return min;
}

void BoardGraphs::narrow_moves(const std::vector<index_t> & indexes) {
    _boxdep_moves = _all_moves;
    for (const auto ind: indexes) {
        _boxdep_moves.remove_node(ind);
    }
}

flags BoardGraphs::narrowed_moves_bitset(const index_t from) const {
    bitset<MAX_TILE_COUNT> result;

    auto nodes = _boxdep_moves.nodes();
    for_each(nodes.begin(from), nodes.end(),
             [&result](auto ind){ result[ind] = true; });

    return result;
}

