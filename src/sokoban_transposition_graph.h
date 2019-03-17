#ifndef SOKOBAN_TRANSPOSITION_GRAPH_H
#define SOKOBAN_TRANSPOSITION_GRAPH_H

#include <unordered_set>
#include <utility>
#include <iostream>
#include <optional>

#include "sokoban_common.h"
#include "sokoban_boxstate.h"

namespace Sokoban
{

class TranspositionGraph {
    /* using NodeType = Node<unsigned short, PushType>; */
    /* NodeType * root; */
    using GValue = std::pair<unsigned, PushType>;
    std::vector<std::vector<GValue>> graph;
    std::vector<bool> state_status;

public:
    TranspositionGraph() : graph{}, state_status{} {
        graph.reserve(15000);
        state_status.reserve(15000);
    }

    void insert_state(unsigned base_state_id, unsigned new_state_id, PushType pt, bool is_complete) {
        if (graph.size() <= base_state_id) {
            graph.resize(base_state_id + 1);
        }
        if (graph.size() <= new_state_id) {
            graph.resize(new_state_id + 1);
        }

        state_status[new_state_id] = is_complete;
        graph[base_state_id].push_back(make_pair(new_state_id, pt));
    }

    size_t count() const {
        return graph.size();
    }

    std::optional<std::vector<PushType>> get_path() const {
        std::vector<bool> visited(graph.size(), false);
        std::vector<PushType> path;

        return print_result_rec(visited, path, 0, PushType(), static_cast<unsigned>(graph.size() - 1));
    }

    std::optional<std::vector<PushType>>
    print_result_rec(std::vector<bool> & visited, std::vector<PushType> & path,
                     unsigned index, PushType pt, unsigned dest) const {
        visited[index] = true;
        if (index != 0) { path.push_back(pt); }

        if (index == dest) {
            return path;
        } else {
            for (const auto & [nind, pt]: graph[index]) {
                if (!visited[nind]) {
                    auto res = print_result_rec(visited, path, nind, pt, dest);
                    if (res.has_value()) { return res; }
                }
            }
        }

        path.pop_back();
        visited[index] = false;
        return std::nullopt;
    }

    void print() const {
        for (size_t i = 0; i < graph.size(); i++) {
            std::cout << "st." << std::setw(3) << i << " ";
            if (state_status[i]) { std::cout << "(completed)"; }
            std::cout << ":\n";

            for (const auto [ind, pt]: graph[i]) {
                std::cout << "    -> st." << std::setw(3) << ind;
                const auto [i1, i2, d] = pt;
                std::cout << "; ind." << std::setw(3) << i1
                          << " -> ind." << std::setw(3) << i2 << ";\n";
            }
            /* std::cout << std::endl; */
        }
    }
};

}

#endif
