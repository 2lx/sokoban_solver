#ifndef DEADLOCK_INFO_H
#define DEADLOCK_INFO_H

#include <vector>
#include <ostream>
#include <istream>

namespace Sokoban
{

struct Point {
    int x;
    int y;
};

inline bool operator<(const Point & l, const Point & r) {
    return l.y < r.y || (l.y == r.y && l.x < r.x);
}

inline bool operator==(const Point & l, const Point & r) {
    return l.y == r.y && l.y == r.y;
}

inline Point offset(size_t goali, size_t roomi, size_t width) {
    Point c;
    c.x = static_cast<int>(roomi % width) - static_cast<int>(goali % width);
    c.y = static_cast<int>(roomi / width) - static_cast<int>(goali / width);

    return c;
}


struct DeadlockInfo {
    std::vector<Point> walls;
    std::vector<Point> spaces;
    std::vector<Point> boxes;
    std::vector<std::vector<Point>> goalsets; // solvable combinations
    bool independent_of_goals;
};

inline bool operator==(const DeadlockInfo & l, const DeadlockInfo & r) {
    return l == r;
}

std::ostream & operator<<(std::ostream & stream, const Sokoban::Point & c);
std::ostream & operator<<(std::ostream & stream, const Sokoban::DeadlockInfo & dli);
}

#endif
