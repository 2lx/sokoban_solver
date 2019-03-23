#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

/* #include "sokoban_common.h" */
#include <queue>
#include <utility>

template<typename T>
class PriorityQueue {
private:
    using queue_t = std::queue<T>;
    std::vector<queue_t> _queues;

    size_t max_priority_index() const {
        for (size_t i = _queues.size() - 1; i > 0; i--) {
            if (!_queues[i].empty()) { return i; }
        }
        return 0;
    }

public:
    explicit PriorityQueue(size_t size) : _queues(size) { };

    void push(const size_t priority, const T & data) {
        assert(priority <= _queues.size() - 1);

        _queues[priority].push(data);
    }

    T front() const {
        size_t index = max_priority_index();
        return _queues[index].front();
    }

    void pop() {
        size_t index = max_priority_index();
        _queues[index].pop();
    }

    bool empty() const {
        return max_priority_index() == 0 && _queues[0].empty();
    }
};

#endif
