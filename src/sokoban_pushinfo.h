#ifndef SOKOBAN_PUSHINFO_H
#define SOKOBAN_PUSHINFO_H

#include "sokoban_common.h"
#include <ostream>

namespace Sokoban
{

enum class Direction : unsigned char {
    Up, Left, Right, Down,
    Count
};

static constexpr size_t DIR_COUNT = static_cast<size_t>(Direction::Count);

constexpr Direction direction(const index_t indfrom, const index_t indto) noexcept {
    if (indfrom < indto) {
        if (indfrom + 1 == indto) { return Direction::Right; }
        else { return Direction::Down; }
    } else {
        if (indfrom == indto + 1) { return Direction::Left; }
        else { return Direction::Up; }
    }
}

constexpr char to_char(const Direction d) noexcept {
    switch (d) {
        case Direction::Up:    return 'U';
        case Direction::Left:  return 'L';
        case Direction::Right: return 'R';
        case Direction::Down:  return 'D';
        default:               return ' ';
    }
}

class PushInfo {
    index_t _from;
    index_t _to;
public:
    PushInfo(index_t from, index_t to) : _from(from), _to(to) { };

    index_t from() const { return _from; }
    index_t to()   const { return _to;   }
};

inline std::ostream & operator<<(std::ostream & str, const PushInfo & pi) {
    Direction dir = direction(pi.from(), pi.to());
    return str << pi.from() << ':' << to_char(dir);
}

}

#endif
