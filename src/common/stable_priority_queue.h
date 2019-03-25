// std::priority_queue is implemented using a binary heap, so if you insert
// some elements whose priorities are equal, they may not come out in the
// order of insertion. This class is implemented to fix this problem
// Also, for cases when we are known the maximum priority value in advance,
// we can even implement the priority queue without a binary heap.

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <vector>
#include <deque>
#include <utility>

template<typename T>
class StablePriorityQueue {
private:
    using queue_t = std::deque<T>;
    std::vector<queue_t> _queues;

    size_t max_priority_index() const {
        for (size_t i = _queues.size() - 1; i > 0; --i) {
            if (!_queues[i].empty()) { return i; }
        }
        return 0;
    }

public:
    explicit StablePriorityQueue(size_t size) : _queues(size) { };

    void push(const size_t priority, const T & data) {
        assert(priority <= _queues.size() - 1);

        _queues[priority].push_back(data);
    }

    T front() const {
        const size_t index = max_priority_index();

        assert(!_queues[index].empty());

        return _queues[index].front();
    }

    void pop() {
        const size_t index = max_priority_index();

        assert(!_queues[index].empty());

        _queues[index].pop_front();
    }

    bool empty() const {
        return max_priority_index() == 0 && _queues[0].empty();
    }
};

#endif
