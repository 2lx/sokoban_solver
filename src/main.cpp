#include <iostream>
#include "sokoban_solver.h"
#include "zobrist_hash.h"

using namespace std;

int main() {
    Sokoban::Solver solver;
    if (!solver.read_level_data(cin)) {
        cout << "Invalid input data" << endl;
        return EXIT_FAILURE;
    }

    if (solver.solve()) {
        solver.print_solution(cout);
    }
}
