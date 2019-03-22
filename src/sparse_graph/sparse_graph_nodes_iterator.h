#ifndef SPARSE_GRAPH_NODES_ITERATOR_H
#define SPARSE_GRAPH_NODES_ITERATOR_H

#include <iterator>

template <typename T, size_t ADJ_MAX, bool Directed>
class SparseGraph;

template <typename T, size_t ADJ_MAX, bool Directed, bool CalcDistances>
class SparseGraphNodes;

template <typename T, size_t ADJ_MAX, bool Directed, bool CalcDistances>
class SparseGraphNodesIterator {
    SparseGraphNodes<T, ADJ_MAX, Directed, CalcDistances> * _graph_nodes;

public:
    SparseGraphNodesIterator(SparseGraphNodes<T, ADJ_MAX, Directed, CalcDistances> & gn)
        : _graph_nodes{ &gn } { }

    SparseGraphNodesIterator() : _graph_nodes{nullptr} { }

    ~SparseGraphNodesIterator() { }

    T operator *() const {
        if constexpr (CalcDistances) {
            return _graph_nodes->_queue.front().second;
        } else {
            return _graph_nodes->_queue.front();
        }
    }

    SparseGraphNodesIterator & operator ++() {
        _graph_nodes->iterate();
        return *this;
    }

    bool operator !=(const SparseGraphNodesIterator &) const {
        return !_graph_nodes->_queue.empty();
    }

    bool operator ==(const SparseGraphNodesIterator & other) const {
        return !(*this != other);
    }
};

namespace std
{
template <typename T, size_t ADJ_MAX, bool Directed, bool CalcDistances>
struct iterator_traits<SparseGraphNodesIterator<T, ADJ_MAX, Directed, CalcDistances>> {
    using iterator_category = std::input_iterator_tag;
    using value_type        = T;
};

}

#endif
