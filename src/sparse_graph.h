#ifndef SPARSE_GRAPH_H
#define SPARSE_GRAPH_H

#include <array>
#include <vector>
#include <limits>
#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>

template <typename Node, size_t ADJ_MAX, bool Directed = false>
class SparseGraph {
private:
    std::vector<std::array<Node, ADJ_MAX>> edges;

public:
    static constexpr Node EMPTY = std::numeric_limits<Node>::max();

    SparseGraph() : edges{} {
        edges.reserve(500);
    }

    SparseGraph(const SparseGraph &) = default;

    void set_node_count(size_t count) {
        edges.resize(count);
        for (auto & edge: edges) {
            for (auto & i: edge) { i = EMPTY; }
        }
    }

    void remove_vertice_edges(Node ind) {
        for (auto & e: edges[ind]) {
            if (e != EMPTY) {
                for (auto & re: edges[e]) {
                    if (re == ind) { re = EMPTY; break; }
                }
                e = EMPTY;
            }
        }
    }

    void add_edge(Node ind1, Node ind2) {
        for (auto & e: edges[ind1]) {
            if (e == ind2)  { break; }
            if (e == EMPTY) { e = ind2; break; }
        }

        if constexpr (!Directed) {
            for (auto & e: edges[ind2]) {
                if (e == ind1)  { break; }
                if (e == EMPTY) { e = ind1; break; }
            }
        }
    }

    void recursive_DFS(std::queue<Node> & q, std::vector<bool> & visited) const {
        auto t = q.front();
        q.pop();

        for (const auto node: edges[t]) {
            if (node == EMPTY || visited[node]) { continue; }

            q.push(node);
            visited[node] = true;
        }
    }

    Node find_adjacent_min(Node start) const {
        std::queue<Node> q;
        std::vector<bool> visited(edges.size(), false);

        q.push(start);
        visited[start] = true;

        Node cmin = start;
        while (!q.empty()) {
            if (cmin > q.front()) { cmin = q.front(); }
            recursive_DFS(q, visited);
        }
        return cmin;
    }

    std::vector<Node> get_joined(Node start) const {
        std::vector<Node> result;
        std::queue<Node> q;
        std::vector<bool> visited(edges.size(), false);

        q.push(start);
        result.push_back(start);
        visited[start] = true;

        while (!q.empty()) {
            result.push_back(q.front());
            recursive_DFS(q, visited);
        }
        return result;
    }


    bool is_joined(Node start, Node finish) const {
        std::queue<Node> q;
        std::vector<bool> visited(edges.size(), false);

        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            if (finish == q.front()) { return true; }
            recursive_DFS(q, visited);
        }
        return false;
    }

    void remove_impassable(std::vector<Node> & goals) {
        std::queue<Node> q;
        std::vector<bool> visited(edges.size(), false);

        for (const auto ind: goals) {
            q.push(ind);
            visited[ind] = true;
        }

        while (!q.empty()) {
            recursive_DFS(q, visited);
        }

        for (size_t i = 0; i < edges.size(); i++) {
            if (visited[i]) { continue; }
            for (auto & e: edges[i]) { e = EMPTY; }
        }
    }

    void transpose() {
        std::vector<std::array<Node, ADJ_MAX>> tedges(edges.size());
        for (auto & edge: tedges) {
            for (auto & i: edge) { i = EMPTY; }
        }

        for (size_t i = 0u; i < edges.size(); i++) {
            for (auto & ind: edges[i]) {
                if (ind != EMPTY) {
                    for (auto & e: tedges[ind]) {
                        if (e == EMPTY) { e = static_cast<Node>(i); break; }
                    }
                }
            }
        }

        edges = tedges;
    }

    const std::array<Node, ADJ_MAX> & siblings(size_t index) const {
        return edges[index];
    }

    void print() const {
        for (size_t k = 0; k < edges.size(); k++) {
            if (edges[k][0] == EMPTY) { continue; }

            std::cout << std::setw(3) << k << ": ";
            for (const auto i: edges[k]) {
                if (i == EMPTY) { continue; }
                std::cout << std::setw(4) << i;
            }
            std::cout << '\n';
        }
    }
};

#endif
