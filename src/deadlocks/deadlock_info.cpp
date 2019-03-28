#include "deadlock_info.h"
#include <string>
#include "string_join.h"
#include <iomanip>

using namespace Sokoban;
using namespace std;

namespace Sokoban
{
ostream & operator<<(ostream & stream, const Point & c) {
    return stream << "{"
                  << setw(2) << c.x
                  << ", "
                  << setw(2) << c.y
                  << "}";
}

ostream & operator<<(ostream & stream, const DeadlockInfo & dli) {
    const string comment_pattern = "\n//     \n//     \n//    ?";
    const size_t comment_line_length = comment_pattern.length() / 3;

    auto format = [cll=static_cast<int>(comment_line_length)]
        (string & str, const Sokoban::Point & p, char ch) {
            const int index = cll*3 - 1 + p.y*cll + p.x;
            if (   (ch == '.' && str[index] == '$') || (ch == '$' && str[index] == '.')) {
                ch = '*';
            }
            str[index] = ch;
    };

    const auto & [ws, ss, bs, gs, flag] = dli;
    string comment{ comment_pattern };

    // format the comment
    for (auto p: ws) { format(comment, p, '#'); }
    for (auto p: bs) { format(comment, p, '$'); }
    if (!flag) { for (auto p: gs) { format(comment, p, '.'); } }
    /* if (flag) { comment += "   goal independent"; } */

    return stream << comment
                  << "\n  { "
                  << "{ " << string_join(ws, ", ") << " }, "
                  << "{ " << string_join(ss, ", ") << " }, "
                  << "{ " << string_join(bs, ", ") << " }, "
                  << "{ " << string_join(gs, ", ") << " }, "
                  << (flag ? 1 : 0)
                  << " }";
}
}
