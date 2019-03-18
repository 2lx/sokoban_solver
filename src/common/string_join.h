#ifndef JOIN_H__
#define JOIN_H__
// https://stackoverflow.com/questions/5288396/c-ostream-out-manipulation

#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>

// join all items in a container into a string, using a 'delim' as separator
template <typename Range, typename Value = typename Range::value_type>
std::string join(const Range & elements, const char * const delim = " ") {
    std::ostringstream os;
    auto first = begin(elements),
         last  = end(elements);

    if (first != last) {
        auto plast = prev(last);
        std::copy(first, plast, std::ostream_iterator<Value>(os, delim));
        os << *plast;
    }

    return os.str();
}

#endif
