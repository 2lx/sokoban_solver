#ifndef JOIN_H__
#define JOIN_H__
// https://stackoverflow.com/questions/5288396/c-ostream-out-manipulation

#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>

// join all items in a container into a string, using a 'delim' as separator
template <typename Range, typename Value = typename Range::value_type>
std::string string_join(const Range & elements, const char * const delim = " ") {
    std::ostringstream os;
    auto first = std::begin(elements),
         last  = std::end(elements);

    if (first != last) {
        auto plast = std::prev(last);
        std::copy(first, plast, std::ostream_iterator<Value>(os, delim));
        os << *plast;
    }

    return os.str();
}

#endif
