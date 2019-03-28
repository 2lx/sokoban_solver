// The implementation of graph structure.
// Represents the graph, which nodes are of integral type T and can have
// at most ADJ_MAX edges. Graph may be directed and undirected,
// it depends on parameter Directed.
// Graph is presented in memory as an adjacency list.

#ifndef SPARSE_GRAPH_H
#define SPARSE_GRAPH_H

#include <vector>
#include <array>
#include <limits>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>

template <typename T, size_t ADJ_MAX, bool Directed, bool CalcDistances>
class SparseGraphNodes;

template <typename T, size_t ADJ_MAX, bool Directed, bool CalcDistances>
class SparseGraphNodesIterator;

template <typename T, size_t ADJ_MAX, bool Directed>
class SparseGraphEdgesIterator;

template <typename T, size_t ADJ_MAX, bool Directed>
class SparseGraph {
private:
    friend class SparseGraphNodes<T, ADJ_MAX, Directed, true>;
    friend class SparseGraphNodes<T, ADJ_MAX, Directed, false>;
    friend class SparseGraphEdgesIterator<T, ADJ_MAX, Directed>;

    std::vector<std::array<T, ADJ_MAX>> _edges;

    // generates the blank node structure at compile time
    static constexpr std::array<T, ADJ_MAX> blank_array() {
        std::array<T, ADJ_MAX> blank{};
        for (auto & b: blank) { b = EMPTY; }
        return blank;
    }

    // Removes the reference to the edge between <from> and <to> nodes from <from>'s
    // adjacency list. If there is exists reference to that edge in <to>'s adjacency
    // list, keeps it.
    void remove_edge_one_way(T from, T to) {
        auto it = std::find(std::begin(_edges[from]), std::end(_edges[from]), to);
        if (it != end(_edges[from])) { *it = EMPTY; }
    };

    // insert the edge between <from> and <to> nodes
    void insert_edge_one_way(T from, T to) {
        if (std::find(begin(_edges[from]), end(_edges[from]),
                      to) != end(_edges[from])) return;

        auto it = std::find(begin(_edges[from]), end(_edges[from]), EMPTY);

        // check if there are more adjacency nodes than ADJ_MAX
        assert(it != end(_edges[from]));

        *it = to;
    };


public:
    static constexpr T EMPTY = std::numeric_limits<T>::max();
    static constexpr std::array<T, ADJ_MAX> BLANK = blank_array();

    SparseGraph() : _edges{} {
        _edges.reserve(1000);
    }

    SparseGraph(const SparseGraph &) = default;
    SparseGraph(SparseGraph &&) = default;
    SparseGraph & operator=(const SparseGraph &) = default;
    SparseGraph & operator=(SparseGraph &&) = default;

    // Returns an iterable object containing information about graph nodes
    auto nodes() const {
        return SparseGraphNodes<T, ADJ_MAX, Directed, false>(*this);
    }

    // Returns an iterable object containing information about graph nodes.
    // During the iteration the object calculates the distances between nodes
    // by the shortest path length
    auto nodes_with_distances() const {
        return SparseGraphNodes<T, ADJ_MAX, Directed, true>(*this);
    }

    // Returns the count of the nodes in graph
    size_t size() const noexcept { return _edges.size(); }

    // Sets the count of the nodes in graph. If the new size is larger
    // than the old one, the new nodes are filled as isolated (they have no edges).
    // This operation is equivalent to adding all nodes at once
    void resize(size_t count) {
        _edges.resize(count, BLANK);
    }

    // removes all edges adjacent (in both directions) to a certain node
    void remove_node(T index) {
        assert(index < _edges.size());

        for (auto & e: _edges[index]) {
            if (e == EMPTY) { continue; }

            if constexpr (!Directed) { remove_edge_one_way(e, index); }
            e = EMPTY;
        }

        if constexpr (Directed) {
            for (T from = 0; from < _edges.size(); ++from) {
                remove_edge_one_way(from, index);
            }
        }
    }

    // Insert an edge (two edges in the case of an undirected graph) between
    // certain nodes
    void insert_edge(T from, T to) {
        assert(from < _edges.size() && to < _edges.size());

        insert_edge_one_way(from, to);
        if constexpr (!Directed) {
            insert_edge_one_way(to, from);
        }
    }

    // Remove the edge between two nodes. In the undirected graph,
    // both edge marks are removed
    void remove_edge(T from, T to) {
        assert(from < _edges.size() && to < _edges.size());

        remove_edge_one_way(from, to);
        if constexpr (!Directed) {
            remove_edge_one_way(to, from);
        }
    };

    // returns signal flags, indicated which of the nodes are available from the start node
    std::vector<bool> test_passability(const T start, const std::vector<T> & nodes) const {
        auto grnodes = this->nodes();
        std::for_each(grnodes.begin(start), grnodes.end(), [](auto){}); // just iterate

        std::vector<bool> result(nodes.size(), false);
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (grnodes.visited(nodes[i])) { result[i] = true; }
        }
        return result;
    }

    // Removes all edges (in both directions) for all nodes from the <indexes>.
    // !currently implemented only for directed graphs
    void remove_impassable(std::vector<T> & indexes) {
        // implemented only for directed graphs
        if constexpr (!Directed) return;

        auto nodes = this->nodes();
        std::for_each(nodes.begin(indexes), nodes.end(), [](auto){}); // just iterate

        for (size_t i = 0; i < _edges.size(); i++) {
            if (!nodes.visited(i)) { _edges[i] = BLANK; }
        }
    }

    // Transposes this graph
    void transpose() {
        // if graph is not directed, it is already symmetric
        if constexpr (!Directed) { return; }

        SparseGraph<T, ADJ_MAX, Directed> newgraph;
        newgraph.resize(_edges.size());

        for (size_t i = 0u; i < _edges.size(); i++) {
            for (auto & ind: _edges[i]) {
                if (ind != EMPTY) {
                    newgraph.insert_edge(ind, static_cast<T>(i));
                }
            }
        }

        std::swap(*this, newgraph);
    }

    // Returns the iterator through nodes adjacent to <index> node
    auto edges_begin(size_t index) const {
        return SparseGraphEdgesIterator<T, ADJ_MAX, Directed>(*this, index);
    }

    auto edges_end() const {
        return SparseGraphEdgesIterator<T, ADJ_MAX, Directed>();
    }

    // Prints the structure of this graph
    void print() const {
        using namespace std;
        for (size_t i = 0; i < _edges.size(); i++) {
            if (edges_begin(i) == edges_end()) { continue; }

            cout << setw(3) << i << ": ";
            for (auto it = edges_begin(i); it != edges_end(); ++it) {
                cout << setw(4) << *it;
            }
            cout << '\n';
        }
    }
};

#include "sparse_graph_nodes.h"
#include "sparse_graph_nodes_iterator.h"
#include "sparse_graph_edges_iterator.h"

#endif
