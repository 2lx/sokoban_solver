#ifndef REPLACE_SORTED_H
#define REPLACE_SORTED_H

#include <algorithm>

// replaces the first occurence of old_value in a sorted container
// with new_value and sorts the resulting container
template<class Iter, class T>
Iter replace_sorted(Iter first, Iter last, const T & old_value, const T & new_value)
{
    Iter pos = std::find(first, last, old_value);

    while (next(pos) != last  && new_value > *next(pos)) {
        *pos = *next(pos); ++pos;
    }
    while (     pos  != first && new_value < *prev(pos)) {
        *pos = *prev(pos); --pos;
    }

    *pos = new_value;
    return pos;
}

#endif
