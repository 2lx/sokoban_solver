#ifndef SPARSE_GRAPH_NODES_H
#define SPARSE_GRAPH_NODES_H

#include <queue>
#include <vector>
#include <utility>
#include <type_traits>
#include <functional>
#include <cassert>
#include <algorithm>

template <typename T, size_t ADJ_MAX, bool Directed>
class SparseGraph;

template <typename T, size_t ADJ_MAX, bool Directed, bool CalcDistances>
class SparseGraphNodesIterator;

template <typename T, size_t ADJ_MAX, bool Directed, bool CalcDistances>
class SparseGraphNodes {
    friend class SparseGraph<T, ADJ_MAX, Directed>;
    friend class SparseGraphNodesIterator<T, ADJ_MAX, Directed, CalcDistances>;

    using queue_t = std::queue<std::conditional_t<CalcDistances == true, std::pair<size_t, T>, T>>;
    using distances_t = std::conditional_t<CalcDistances == true, std::vector<size_t>, size_t>;

    const SparseGraph<T, ADJ_MAX, Directed> & _graph;
    queue_t     _queue;
    distances_t _distances;
    std::vector<bool> _visited;
    bool _traversed;

    inline void iterate() {
        _traversed = true;

        T nodei;
        size_t dist;
        if constexpr (CalcDistances) {
            std::tie(dist, nodei) = _queue.front();
            _distances[nodei] = dist;
        } else {
            nodei = _queue.front();
        }
        _queue.pop();

        for (const auto ind: _graph._edges[nodei]) {
            if (   ind == SparseGraph<T, ADJ_MAX, Directed>::EMPTY
                || _visited[ind]) { continue; }

            if constexpr (CalcDistances) {
                _queue.push(std::make_pair(dist + 1u, ind));
            } else {
                _queue.push(ind);
            }
            _visited[ind] = true;
        }
    }

    explicit SparseGraphNodes(const SparseGraph<T, ADJ_MAX, Directed> & g)
        : _graph{g}, _queue{}, _distances{},
          _visited(_graph._edges.size(), false), _traversed{false} {
            if constexpr (CalcDistances) {
                _distances.resize(_graph._edges.size(), MAXDISTANCE);
            }
    }

    void recover_state() {
        if (_traversed) {
            queue_t empty;
            _queue.swap(empty);
            std::fill(std::begin(_visited), std::end(_visited), false);

            if constexpr (CalcDistances) {
                std::fill(std::begin(_distances), std::end(_distances), MAXDISTANCE);
            }

            _traversed = false;
        }
    }

public:
    static constexpr size_t MAXDISTANCE = std::numeric_limits<size_t>::max();

    auto begin(const std::vector<T> & indexes) {
        recover_state();

        for (const auto index: indexes) {
            assert(index < _graph.size());

            if constexpr (CalcDistances) {
                _queue.push(std::make_pair(0u, index));
            } else {
                _queue.push(index);
            }
            _visited[index] = true;
        }

        return SparseGraphNodesIterator<T, ADJ_MAX, Directed, CalcDistances>(*this);
    }

    bool visited(size_t index) const {
        return _visited[index];
    }

    auto begin(T index) {
        recover_state();

        assert(index < _graph.size());

        if constexpr (CalcDistances) {
            _queue.push(std::make_pair(0u, index));
        } else {
            _queue.push(index);
        }
        _visited[index] = true;

        return SparseGraphNodesIterator<T, ADJ_MAX, Directed, CalcDistances>(*this);
    }

    SparseGraphNodesIterator<T, ADJ_MAX, Directed, CalcDistances> end() const {
        return {};
    }

    const distances_t & distances() const {
        return _distances;
    }
};

#endif
