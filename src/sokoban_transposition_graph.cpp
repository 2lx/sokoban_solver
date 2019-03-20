#include "sokoban_transposition_graph.h"
#include "string_join.h"

#include <iomanip>
#include <ostream>

using namespace std;

namespace Sokoban {
TranspositionGraph::TranspositionGraph() : graph{} {
    graph.reserve(1000000);
    graph.push_back({});
}

void TranspositionGraph::insert_state(stateid_t base_state_id,
        stateid_t new_state_id, PushInfo pt) {
    graph.push_back({});
    graph[base_state_id].emplace_back(new_state_id, pt);
}

std::optional<std::vector<PushInfo>> TranspositionGraph::get_path() const {
    std::vector<PushInfo> path;
    // the destination stateid is always the last inserted one
    const auto dest = static_cast<stateid_t>(graph.size() - 1);

    // graph is acyclic, so we don't need 'visited' flags
    if (collect_path(path, 0u, dest)) { return path; }
    return std::nullopt;
}

bool TranspositionGraph::collect_path(std::vector<PushInfo> & path,
                                      stateid_t index, stateid_t dest) const {
    if (index == dest) { return true; }

    for (const auto & [next_state_id, pushinfo]: graph[index]) {
        path.push_back(pushinfo);
        if (collect_path(path, next_state_id, dest)) { return true; }
        path.pop_back();
    }
    return false;
}

std::ostream & operator<<(std::ostream & stream, const TranspositionGraph::GValue & gv) {
    return stream << "-> st. " << std::setw(3) << gv.stateid
                    << " (" << gv.pushinfo << ")";
}

void TranspositionGraph::print(std::ostream & stream) const {
    for (size_t i = 0; i < graph.size(); i++) {
        stream << "st." << setw(3) << i << " ";
        if (i == graph.size() - 1) { stream << "(completed)"; }
        stream << ':' << join(graph[i], ", ") << '\n';
    }
}
}
