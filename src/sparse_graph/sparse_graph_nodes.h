#ifndef SPARSE_GRAPH_NODES_H
#define SPARSE_GRAPH_NODES_H

#include <queue>
#include <vector>
#include <cassert>
#include <algorithm>

template <typename T, size_t ADJ_MAX, bool Directed>
class SparseGraph;

template <typename T, size_t ADJ_MAX, bool Directed>
class SparseGraphNodesIterator;

template <typename T, size_t ADJ_MAX, bool Directed>
class SparseGraphNodes {
    friend class SparseGraphNodesIterator<T, ADJ_MAX, Directed>;
    friend class SparseGraph<T, ADJ_MAX, Directed>;

    const SparseGraph<T, ADJ_MAX, Directed> & _graph;
    std::queue<T> _queue;
    std::vector<bool> _visited;
    bool _traversed;

    inline void iterate() {
        _traversed = true;
        auto t = _queue.front();
        _queue.pop();

        for (const auto ind: _graph._edges[t]) {
            if (   ind == SparseGraph<T, ADJ_MAX, Directed>::EMPTY
                || _visited[ind]) { continue; }

            _queue.push(ind);
            _visited[ind] = true;
        }
    }

    explicit SparseGraphNodes(const SparseGraph<T, ADJ_MAX, Directed> & g)
        : _graph{g}, _queue{}, _visited(_graph._edges.size(), false), _traversed{false} {
    }

    void recover_state() {
        if (_traversed) {
            std::queue<T> empty;
            _queue.swap(empty);
            std::fill(std::begin(_visited), std::end(_visited), false);

            _traversed = false;
        }
    }

public:
    auto begin(const std::vector<T> & indexes) {
        recover_state();

        for (const auto index: indexes) {
            assert(index < _graph.size());

            _queue.push(index);
            _visited[index] = true;
        }

        return SparseGraphNodesIterator<T, ADJ_MAX, Directed>(*this);
    }

    bool visited(size_t index) const {
        return _visited[index];
    }

    auto begin(T index) {
        recover_state();

        assert(index < _graph.size());

        _queue.push(index);
        _visited[index] = true;

        return SparseGraphNodesIterator<T, ADJ_MAX, Directed>(*this);
    }

    SparseGraphNodesIterator<T, ADJ_MAX, Directed> end() {
        return {};
    }
};

#endif
