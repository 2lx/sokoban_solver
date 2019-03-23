#ifndef SOKOBAN_PRIORITY_QUEUE_H
#define SOKOBAN_PRIORITY_QUEUE_H

#include "sokoban_common.h"
#include <queue>
#include <utility>

namespace Sokoban
{
class PriorityQueue {
private:
    using queue_t = std::queue<std::pair<stateid_t, BoxState>>;
    queue_t _queuepos, _queuezero, _queueneg;

public:
    PriorityQueue() = default;

    void push(stateid_t state_id, const BoxState & bs, const int priority) {
        if      (priority > 0)  { _queuepos.push({ state_id, bs });  }
        else if (priority == 0) { _queuezero.push({ state_id, bs }); }
        else                    { _queueneg.push({ state_id, bs });  }
    }

    auto front() const {
        if      (!_queuepos.empty())  { return _queuepos.front();  }
        else if (!_queuezero.empty()) { return _queuezero.front(); }
        else                          { return _queueneg.front(); }
    }

    void pop() {
        if      (!_queuepos.empty())  { return _queuepos.pop();  }
        else if (!_queuezero.empty()) { return _queuezero.pop(); }
        else                          { return _queueneg.pop(); }
    }

    bool empty() const {
        return _queuepos.empty() && _queuezero.empty() && _queueneg.empty();
    }
};
}

#endif
