#include "sokoban_solver.h"
#include "sokoban_formatter.h"

#include "sokoban_pushinfo.h"
#include "string_join.h"
#include "priority_queue.h"

#include <iterator>
#include <iostream>
#include <string>

using namespace std;
using namespace Sokoban;

bool Solver::read_level_data(std::istream & stream) {
    string line;
    vector<Tile> maze;
    maze.reserve(MAX_TILE_COUNT);

    ushort width = 0, height = 0;
    while(getline(stream, line) && !line.empty()) {
        if (width == 0) { width = static_cast<unsigned short>(line.length()); }
        else if (width != line.length()) { return false; }

        for (const auto ch: line) {
            const auto tile = Formatter::encode(ch);
            if (!tile.has_value()) { return false; }
            maze.push_back(tile.value());
        }
        height++;
    }

    return board.initialize(move(maze), width, height);
}

void Solver::print_solution(std::ostream & stream) const {
    // board.print_graphs();
    /* tgraph.print(cout); */
    /* ttable.print(); */

    auto path = tgraph.get_path();
    if (path.has_value()) {
        stream << string_join(path.value(), " ") << endl;
    }
}

size_t Solver::max_priority() const {
    return board.box_count() + 4;
}

size_t Solver::calculate_priority(const Board::StateStats & stats) const {
    size_t bos = stats.boxes_on_goals_count;
    size_t ord_bos = stats.ordered_boxes_on_goals_count;
    size_t priority = 0u;
    if      (stats.push_distances.first > stats.push_distances.second) { priority = 4u; }
    else if (stats.push_distances.first < stats.push_distances.second) { priority = 0u; }
    else { priority = 2u; }
    return priority + ord_bos;
}

bool Solver::solve() {
    assert(board.box_count() <= MAX_BOX_COUNT);
    BoxState::set_box_count(board.box_count());

    PriorityQueue<pair<stateid_t, BoxState>> q(max_priority() + 1);
    auto base_state = board.current_state();
    auto [inserted, base_state_id] = ttable.insert_state(base_state);
    q.push(0u, {base_state_id, base_state});

    while (!q.empty()) {
        auto [state_id, state] = q.front();
        q.pop();

        board.set_boxstate(state);
        auto pushes = board.possible_pushes();

        for (const auto [pushinfo, stats]: pushes) {
            board.set_boxstate_and_push(state, pushinfo);

            auto new_state = board.current_state();
            auto [inserted, new_state_id] = ttable.insert_state(new_state);

            if (inserted) {
                tgraph.insert_state(state_id, new_state_id, pushinfo);

                size_t priority = calculate_priority(stats);
                /* cout << endl; */
                /* cout << "priority: " << priority << endl; */
                /* cout << "push: " << pushinfo.from() << ' ' << pushinfo.to() */
                /*      << " bos=" << stats.boxes_on_goals_count */
                /*      << " obos=" << stats.ordered_boxes_on_goals_count */
                /*      << " push_dist=" << stats.push_distances.first */
                /*                       << ' ' << stats.push_distances.second */
                /*      << endl; */
                /* board.print_state(); */

                q.push(priority, {new_state_id, new_state});
                if (board.is_complete()) { return true; }
            };
        }
    }
    return false;
}

