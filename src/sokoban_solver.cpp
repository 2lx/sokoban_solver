#include "sokoban_solver.h"
#include "sokoban_formatter.h"

#include "sokoban_pushinfo.h"
#include "string_join.h"
#include "stable_priority_queue.h"

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

    return _board.initialize(move(maze), width, height);
}

void Solver::print_information() const {
    _board.print_information();
}

void Solver::print_solution_format1(std::ostream & stream) {
    // _board.print_graphs();
    /* _trans_graph.print(cout); */
    /* _trans_table.print(); */

    cout << "Solution (" << _trans_table.size() << " states):" << endl;
    auto path = _trans_graph.get_path();
    if (path.has_value()) {
        stream << string_join(path.value(), " ") << endl;
    }
}

void Solver::print_solution_format2(std::ostream & stream) {
    cout << "Solution (" << _trans_table.size() << " states):" << endl;
    auto path = _trans_graph.get_path();
    if (path.has_value()) {
        _board.set_boxstate(_base_state);
        _board.print_state();

        for (size_t i = 0; i < path.value().size(); ++i) {
            const PushInfo & pi = path.value()[i];
            _board.set_boxstate_and_push(_board.current_state(), pi);

            stream << pi << '\n';
            _board.print_state();
        }
    }
}

size_t Solver::max_priority() const {
    return _board.box_count() + 4;
}

size_t Solver::calculate_priority(const Board::StateStats & stats) const {
    /* size_t bos = stats.boxes_on_goals_count; */
    size_t ord_bos = stats.ordered_boxes_on_goals_count;
    size_t priority = 0u;
    if      (stats.push_distances.first > stats.push_distances.second) { priority = 2u; }
    else if (stats.push_distances.first < stats.push_distances.second) { priority = 0u; }
    else { priority = 1u; }
    return priority + ord_bos;
}

bool Solver::solve() {
    assert(_board.box_count() <= MAX_BOX_COUNT);
    BoxState::set_box_count(_board.box_count());
    if (_board.is_complete()) { return true; }

    StablePriorityQueue<pair<stateid_t, BoxState>> q(max_priority() + 1);
    _base_state = _board.current_state();
    auto [inserted, base_state_id] = _trans_table.insert_state(_base_state);
    q.push(0u, {base_state_id, _base_state});

    while (!q.empty()) {
        auto [state_id, state] = q.front();
        q.pop();

        _board.set_boxstate(state);
        auto pushes = _board.possible_pushes();

        for (const auto [pushinfo, stats]: pushes) {
            _board.set_boxstate_and_push(state, pushinfo);

            auto new_state = _board.current_state();
            auto [inserted, new_state_id] = _trans_table.insert_state(new_state);

            if (inserted) {
                _trans_graph.insert_state(state_id, new_state_id, pushinfo);

                /* cout << endl; */
                /* cout << "priority: " << priority << endl; */
                /* cout << "push: " << pushinfo.from() << ' ' << pushinfo.to() */
                /*      << " bos=" << stats.boxes_on_goals_count */
                /*      << " obos=" << stats.ordered_boxes_on_goals_count */
                /*      << " push_dist=" << stats.push_distances.first */
                /*                       << ' ' << stats.push_distances.second */
                /*      << endl; */
                /* _board.print_state(); */

                size_t priority = calculate_priority(stats);
                q.push(priority, {new_state_id, new_state});
                if (_board.is_complete()) { return true; }
            };
        }
    }
    return false;
}

