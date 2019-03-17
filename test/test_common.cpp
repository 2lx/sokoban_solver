#include <boost/test/unit_test.hpp>
#include "sokoban_solver.h"

#include <string_view>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

bool test(const string_view & indata, const vector<string_view> & outdata) {
    Sokoban::Solver solver;
    istringstream iss(string{indata});

    bool is_read = solver.read_level_data(iss);
    BOOST_REQUIRE_MESSAGE(is_read == true, "Test failed: Invalid input data");

    auto result = solver.solve();
    BOOST_REQUIRE_MESSAGE(result == true, "Test failed: Solution was not found");

    ostringstream oss;
    solver.print_solution(oss);

    bool test_result = find(begin(outdata), end(outdata), oss.str()) != end(outdata);

    BOOST_REQUIRE_MESSAGE(
        test_result,
         "\nTest failed:\nInput data:\n" << indata
        << "Exptected data:\n" << outdata[0]
        << "Observed data:\n" << oss.str());

    return test_result;
}
