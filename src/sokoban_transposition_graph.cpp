#include "sokoban_transposition_graph.h"
#include "string_join.h"

#include <iomanip>
#include <ostream>
#include <cassert>

using namespace std;

namespace Sokoban {
TranspositionGraph::TranspositionGraph() : _graph{} {
    _graph.reserve(1000000u);
    _graph.push_back({0u, {0u, 0u}});
}

void TranspositionGraph::insert_state(stateid_t base_state_id,
                                      stateid_t new_state_id, PushInfo pi) {
    _graph.push_back({base_state_id, pi});
    assert(new_state_id + 1 == _graph.size());
}

std::optional<std::vector<PushInfo>> TranspositionGraph::get_path() const {
    std::vector<PushInfo> path;
    path.reserve(1000u);

    stateid_t parent_id = _graph.back().stateid;
    path.push_back(_graph.back().pushinfo);

    while (parent_id != 0u) {
        path.push_back(_graph[parent_id].pushinfo);
        parent_id = _graph[parent_id].stateid;
    }

    reverse(begin(path), end(path));
    return path;
}

std::ostream & operator<<(std::ostream & stream, const TranspositionGraph::GValue & gv) {
    return stream << "-> st. " << std::setw(3) << gv.stateid
                    << " (" << gv.pushinfo << ")";
}

void TranspositionGraph::print(std::ostream & stream) const {
    for (size_t i = 0; i < _graph.size(); i++) {
        stream << "st." << setw(3) << i << " ";
        if (i == _graph.size() - 1) { stream << "(completed)"; }
        stream << ':' << _graph[i] << '\n';
    }
}
}
