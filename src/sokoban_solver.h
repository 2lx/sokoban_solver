#ifndef SOKOBAN_SOLVER_H
#define SOKOBAN_SOLVER_H

#include <iosfwd>
#include "sokoban_board.h"
#include "sokoban_transposition_table.h"
#include "sokoban_transposition_graph.h"

namespace Sokoban
{
class Solver {
private:
    Solver(const Solver &) = delete;
    Solver(Solver &&) = delete;
    Solver & operator=(const Solver &) = delete;
    Solver & operator=(Solver &&) = delete;

    Board board;
    TranspositionTable _trans_table;
    TranspositionGraph _trans_graph;
    BoxState _base_state;

    size_t calculate_priority(const Board::StateStats & stats) const;
    size_t max_priority() const;

public:
    Solver() = default;

    bool read_level_data(std::istream & stream);
    bool solve();
    void print_solution_format1(std::ostream & stream);
    void print_solution_format2(std::ostream & stream);
};
}

#endif
