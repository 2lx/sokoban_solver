#include "sokoban_solver.h"
#include "sokoban_formatter.h"

#include <istream>
#include <ostream>
#include <iomanip>
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

    board.initialize(move(maze), width, height);
    return true;
}

void Solver::print_solution(std::ostream & stream) const {
    /* board.print_graphs(); */
    /* tgraph.print(); */
    /* ttable.print(); */

    auto path = tgraph.get_path();
    if (path.has_value()) {
        for (size_t i = 0; i < path.value().size() - 1; ++i) {
        /* for (auto [i1, i2, pd]: path.value()) { */
            auto [i1, i2, pd] = path.value()[i];
            const char chD = to_char(pd);
            stream << i1 << ':' << chD << ' ';
        }
        auto [i1, i2, pd] = path.value().back();
        const char chD = to_char(pd);
        stream << i1 << ':' << chD << endl;
    }
}

bool Solver::solve() {
    queue<pair<unsigned, BoxState>> q;
    auto base_state = board.current_state();
    auto [inserted, base_state_id] = ttable.insert_state(base_state);
    q.push({base_state_id, base_state});

    bool solution_found = false;

    while (!q.empty() && !solution_found) {
        auto [state_id, state] = q.front();
        q.pop();

        board.set_boxstate(state);
        auto pushes = board.possible_pushes();

        for (const auto p: pushes) {
            board.set_boxstate(state);
            board.push(p);

            auto new_state = board.current_state();
            auto [inserted, new_state_id] = ttable.insert_state(new_state);
            if (inserted) {
                q.push({new_state_id, new_state});
                solution_found = board.is_complete();
                tgraph.insert_state(state_id, new_state_id, p, solution_found);
                if (solution_found) { break; }
            };
        }
    }
    return solution_found;
}

