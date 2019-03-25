#define BOOST_TEST_MODULE STABLE_PRIORITY_QUEUE_TESTS

#include <boost/test/unit_test.hpp>
#include "stable_priority_queue.h"
#include <string>
#include <vector>
#include <random>

using namespace std;

template<typename T>
void push_sequence(StablePriorityQueue<T> & spqueue, const vector<pair<size_t, T>> & data) {
    for (const auto & [priority, element]: data) {
        spqueue.push(priority, element);
    }
}

string pop_all(StablePriorityQueue<char> & spqueue) {
    string result;
    while (!spqueue.empty()) {
        auto ch = spqueue.front();
        result.push_back(ch);
        spqueue.pop();
    }
    return result;
}

BOOST_AUTO_TEST_CASE(Test01)
{
    StablePriorityQueue<char> spqueue(5);

    push_sequence(spqueue, {{0, 'A'}, {1, 'B'}, {2, 'C'}, {1, 'D'}, {0, 'E'}});
    BOOST_REQUIRE(pop_all(spqueue) == "CBDAE");

    push_sequence(spqueue, {{2, 'E'}, {2, 'B'}, {2, 'A'}, {2, 'D'}, {2, 'C'}});
    BOOST_REQUIRE(pop_all(spqueue) == "EBADC");

    push_sequence(spqueue, {{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}});
    BOOST_REQUIRE(pop_all(spqueue) == "EDCBA");
}

BOOST_AUTO_TEST_CASE(Test02)
{
    StablePriorityQueue<char> spqueue(10);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> priority_dist(0, 9);
    std::uniform_int_distribution<> letter_dist(0, 25);

    for (int i = 0; i < 20; ++i) {
        vector<pair<size_t, char>> data;
        for (int i = 0; i < 1000; ++i) {
            const size_t priority = priority_dist(gen);
            const char letter   = static_cast<char>('A' + letter_dist(gen));
            data.push_back({priority, letter});
        }
        push_sequence(spqueue, data);
        stable_sort(begin(data), end(data),
            [](const auto & l, const auto & r){ return l.first > r.first; });

        string strexp;
        strexp.reserve(data.size());
        for (const auto & [p, ch]: data) {
            strexp.push_back(ch);
        }

        string strobs = pop_all(spqueue);

        BOOST_REQUIRE_MESSAGE(strobs == strexp,
                "expected: " << strexp << "\nobserved: " << strobs );
    }
}

