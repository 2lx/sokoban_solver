#ifndef SPARSE_GRAPH_EDGES_ITERATOR_H
#define SPARSE_GRAPH_EDGES_ITERATOR_H

#include <iterator>

template <typename T, size_t ADJ_MAX, bool Directed>
class SparseGraph;

template <typename T, size_t ADJ_MAX, bool Directed>
class SparseGraphEdgesIterator {
    const std::array<T, ADJ_MAX> * const _array;
    size_t _index = 0u;

    void skip_empty() noexcept {
        while (_index < ADJ_MAX
            && (*_array)[_index] == SparseGraph<T, ADJ_MAX, Directed>::EMPTY) {
            ++_index;
        }
    }

public:
    SparseGraphEdgesIterator(const SparseGraph<T, ADJ_MAX, Directed> & gn, size_t index)
        : _array{ &gn._edges[index] } {
        skip_empty();
    }

    SparseGraphEdgesIterator() : _array{nullptr} { }

    ~SparseGraphEdgesIterator() { }

    T operator *() const {
        return (*_array)[_index];
    }

    SparseGraphEdgesIterator & operator ++() {
        ++_index;
        skip_empty();
        return *this;
    }

    bool operator !=(const SparseGraphEdgesIterator &) const {
        return _index < ADJ_MAX;
    }

    bool operator ==(const SparseGraphEdgesIterator & other) const {
        return !(*this != other);
    }
};

namespace std
{
template <typename T, size_t ADJ_MAX, bool Directed>
struct iterator_traits<SparseGraphEdgesIterator<T, ADJ_MAX, Directed>> {
    using iterator_category = std::forward_iterator_tag;
    using value_type        = T;
};

}

#endif
