#ifndef SOKOBAN_TRANSPOSITION_GRAPH_H
#define SOKOBAN_TRANSPOSITION_GRAPH_H

#include <vector>
#include <optional>
#include <iosfwd>

#include "sokoban_common.h"
#include "sokoban_pushinfo.h"

namespace Sokoban
{
class TranspositionGraph {
private:
    struct GValue {
        stateid_t stateid;
        PushInfo pushinfo;

        GValue(stateid_t si, PushInfo pi) : stateid(si), pushinfo(pi) { };
        friend std::ostream & operator<<(std::ostream & stream, const GValue & gv);
    };

    std::vector<GValue> _graph;

    bool collect_path(std::vector<PushInfo> & path, stateid_t index, stateid_t dest) const;

public:
    TranspositionGraph();

    void insert_state(stateid_t base_state_id, stateid_t new_state_id, PushInfo pt);
    std::optional<std::vector<PushInfo>> get_path() const;

    void print(std::ostream & stream) const;
    friend std::ostream & operator<<(std::ostream & stream, const TranspositionGraph::GValue & gv);
};
}

#endif
