#ifndef SOKOBAN_BOARD_GRAPHS_H
#define SOKOBAN_BOARD_GRAPHS_H

#include "sokoban_common.h"
#include "sokoban_pushinfo.h"
#include "sparse_graph.h"

#include <vector>

namespace Sokoban
{
class BoardState;

class BoardGraphs {
    using UGraph = SparseGraph<index_t, DIR_COUNT, false>;
    using DGraph = SparseGraph<index_t, DIR_COUNT, true>;

    UGraph _all_moves;
    UGraph _boxdep_moves;

    std::vector<std::vector<index_t>> _boxes_goals;
    std::vector<std::vector<size_t>>  _goals_distances;
    std::vector<DGraph>               _boxes_routes;
    std::vector<size_t>               _goals_order;

    size_t _count, _box_count;

public:
    BoardGraphs() = default;
    bool initialize(const BoardState & state);

    void bipartite_matching(const BoardState & state, const DGraph & reverse_pushes);

    void calculate_routes(const DGraph & reverse_pushes);
    void calculate_goals_distances(const BoardState & state,
                                   const DGraph & reverse_pushes);
    void calculate_goals_order(const BoardState & state,
                               const DGraph & reverse_pushes);

    const auto & route(const size_t ind) const { return _boxes_routes[ind]; }
    const auto & goals(const size_t ind) const { return _boxes_goals[ind]; }
    const auto & distances_to_goal(const size_t goali) const { return _goals_distances[goali]; }
    /* const auto & distance_to_goal_from(size_t goali, size_t ind) const { */
    /*     return _goals_distances[goali][ind]; } */
    const auto & goals_order() const { return _goals_order; }
    int push_distance_diff(const BoardState & state, size_t boxi, const PushInfo & pi) const;

    index_t min_move_index(index_t player) const;

    void narrow_moves(const std::vector<index_t> & indexes);
    flags narrowed_moves_bitset(const index_t from) const;
};
}

#endif
