#include "deadlock_info.h"
#include "deadlock_generator.h"

#include <iostream>

using namespace Sokoban;
using namespace std;

constexpr const char * level_pattern = 1 + R"(
########
#1111  #
# 22   #
#  ??  #
#  ?$  #
# 22   #
#     @#
########
)";

constexpr const char * file_header = 1 + R"(
#include "deadlocks.h"

std::vector<Sokoban::DeadlockInfo> deadlocks2x2 =
{
)";

constexpr const char * file_footer = 1 + R"(
};
)";

int main() {
    DeadlockGenerator dl;
    dl.initialize(level_pattern);
    auto deadlocks = dl.generate();

    cout << file_header;
    for (const auto & dli: deadlocks) {
        cout << dli << ",\n";
    }
    cout << file_footer;
}
