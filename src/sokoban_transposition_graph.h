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
    using GValue = std::pair<unsigned, PushInfo>;
    std::vector<std::vector<GValue>> graph;
    std::vector<bool> state_status;

public:
    TranspositionGraph() : graph{}, state_status{} {
        graph.reserve(15000);
        state_status.reserve(15000);
    }

    void insert_state(unsigned base_state_id, unsigned new_state_id, PushInfo pt, bool is_complete) {
        if (graph.size() <= base_state_id) {
            graph.resize(base_state_id + 1);
        }
        if (graph.size() <= new_state_id) {
            graph.resize(new_state_id + 1);
        }

        state_status[new_state_id] = is_complete;
        graph[base_state_id].emplace_back(new_state_id, pt);
    }

    size_t count() const {
        return graph.size();
    }

    std::optional<std::vector<PushInfo>> get_path() const {
        std::vector<bool> visited(graph.size(), false);
        std::vector<PushInfo> path;

        return print_result_rec(visited, path, 0, PushInfo(0, 0),
                static_cast<stateid_t>(graph.size() - 1));
    }

    std::optional<std::vector<PushInfo>>
    print_result_rec(std::vector<bool> & visited, std::vector<PushInfo> & path,
                     unsigned index, PushInfo pt, unsigned dest) const {
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
                std::cout << "; ind." << std::setw(3) << pt.from()
                          << " -> ind." << std::setw(3) << pt.to() << ";\n";
            }
            /* std::cout << std::endl; */
        }
    }
};

}

#endif
