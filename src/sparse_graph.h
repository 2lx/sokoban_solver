#ifndef SPARSE_GRAPH_H
#define SPARSE_GRAPH_H

#include <array>
#include <limits>
#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>

template <bool directed>
class SparseGraph {
public:
    static constexpr unsigned short EMPTY = std::numeric_limits<unsigned short>::max();

private:
    std::array<std::array<unsigned short, 4>, 250> edges = { 0 };
    unsigned short node_count;

public:
    SparseGraph() : edges{}, node_count{} {
        for (auto & edge: edges) {
            for (auto & i: edge) { i = EMPTY; }
        }
    }

    SparseGraph(const SparseGraph &) = default;

    void set_node_count(unsigned short count) {
        node_count = count;
    }

    void remove_vertice_edges(unsigned short ind) {
        for (auto & e: edges[ind]) {
            if (e != EMPTY) {
                for (auto & re: edges[e]) {
                    if (re == ind) { re = EMPTY; break; }
                }
                e = EMPTY;
            }
        }
    }

    void add_edge(unsigned short ind1, unsigned short ind2) {
        for (auto & e: edges[ind1]) {
            if (e == ind2)  { break; }
            if (e == EMPTY) { e = ind2; break; }
        }

        if constexpr (!directed) {
            for (auto & e: edges[ind2]) {
                if (e == ind1)  { break; }
                if (e == EMPTY) { e = ind1; break; }
            }
        }
    }

    void recursive_DFS(std::queue<unsigned short> & q, std::array<bool, 250> & traversed) const {
        auto t = q.front();
        /* std::cout << "trav: " << t << std::endl; */
        q.pop();

        for (const auto node: edges[t]) {
            if (node == EMPTY || traversed[node]) { continue; }

            q.push(node);
            traversed[node] = true;
        }
    }

    unsigned short find_adjacent_min(unsigned short start) const {
        std::queue<unsigned short> q;
        std::array<bool, 250> traversed = {0};

        q.push(start);
        traversed[start] = true;

        unsigned short cmin = start;
        while (!q.empty()) {
            if (cmin > q.front()) { cmin = q.front(); }
            recursive_DFS(q, traversed);
        }
        return cmin;
    }

    bool is_joined(unsigned short start, unsigned short finish) const {
        std::queue<unsigned short> q;
        std::array<bool, 250> traversed = {0};

        q.push(start);
        traversed[start] = true;

        while (!q.empty()) {
            if (finish == q.front()) { return true; }
            recursive_DFS(q, traversed);
        }
        return false;
    }

    void remove_impassable(std::vector<unsigned short> & goals) {
        std::queue<unsigned short> q;
        std::array<bool, 250> traversed = {0};

        for (const auto ind: goals) {
            q.push(ind);
            traversed[ind] = true;
        }

        while (!q.empty()) {
            recursive_DFS(q, traversed);
        }

        for (unsigned short i = 0; i < node_count; i++) {
            if (traversed[i]) { continue; }
            for (auto & e: edges[i]) { e = EMPTY; }
        }
    }

    void transpose() {
        std::array<std::array<unsigned short, 4>, 250> tedges = { 0 };
        for (auto & edge: tedges) {
            for (auto & i: edge) { i = EMPTY; }
        }

        for (unsigned short i = 0u; i < node_count; i++) {
            for (auto & ind: edges[i]) {
                if (ind != EMPTY) {
                    for (auto & e: tedges[ind]) {
                        if (e == EMPTY) { e = i; break; }
                    }
                }
            }
        }

        edges = tedges;
    }

    std::vector<unsigned short> siblings(size_t index) const {
        std::vector<unsigned short> result;

        for (auto ind: edges[index]) {
            if (ind == EMPTY) { continue; }
            result.push_back(ind);
        }

        return result;
    }

    void print() const {
        for (size_t k = 0; k < node_count; k++) {
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
