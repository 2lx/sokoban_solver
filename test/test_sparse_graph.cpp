#define BOOST_TEST_MODULE SPARSE_GRAPH_TESTS

#include <boost/test/unit_test.hpp>
#include "sparse_graph.h"
#include <string>
#include <list>
#include <algorithm>
#include <random>
#include <functional>

using namespace std;

template <typename T, size_t ADJ_MAX, bool Directed>
string to_string(SparseGraph<T, ADJ_MAX, Directed> & graph) {
    string result;

    for (size_t i = 0; i < graph.size(); ++i) {
        auto it = graph.edges_begin(i);
        while (it != graph.edges_end()) {
            result += to_string(i) + ':' + to_string(*it) + ' ';
            ++it;
        }
    }

    return result;
}

using vpair = pair<size_t, size_t>;

bool pair_equal(const vpair & l, const vpair & r) {
    return l.first == r.first && l.second == r.second;
}

void insert_to_list_uniquely(list<vpair> & ledges, size_t l, size_t r) {
    if (find_if(begin(ledges), end(ledges),
                bind(pair_equal, placeholders::_1, make_pair(l, r))) == end(ledges)) {
        ledges.push_back({l, r});
    }
}

void remove_from_list(list<vpair> & ledges, size_t l, size_t r) {
    if (auto it = find_if(begin(ledges), end(ledges),
                bind(pair_equal, placeholders::_1, make_pair(l, r)));
        it != end(ledges))
    {
        ledges.erase(it);
    }
}

string to_string(list<vpair> & ledges) {
    ledges.sort([](const auto & l, const auto & r){ return l.first < r.first; }); // stable sort

    string result;
    for (const auto & [from, to]: ledges) {
        result += to_string(from) + ':' + to_string(to) + ' ';
    }

    return result;
}

void boost_test(const string & expected, const string & observed) {
    BOOST_REQUIRE_MESSAGE(observed == expected,
               "\nobserved: " << observed
            << "\nexpected: " << expected);
}

std::random_device rd;
std::mt19937 gen(rd());

template <size_t MAX_ADJ, bool Directed>
void test_insertion(SparseGraph<size_t, MAX_ADJ, Directed> & graph, list<vpair> & ledges) {
    std::uniform_int_distribution<> index_dist(0, MAX_ADJ - 1);

    for (size_t i = 0; i < 2*MAX_ADJ; ++i) {
        size_t from = index_dist(gen);
        size_t to   = index_dist(gen);
        if (from == to) continue;

        graph.insert_edge(from, to);

        insert_to_list_uniquely(ledges, from, to);
        if constexpr (!Directed) {
            insert_to_list_uniquely(ledges, to, from);
        }
    }
}

template <size_t MAX_ADJ, bool Directed>
void test_removal(SparseGraph<size_t, MAX_ADJ, Directed> & graph, list<vpair> & ledges) {
    std::uniform_int_distribution<> index_dist(0, MAX_ADJ - 1);

    for (size_t i = 0; i < 10*MAX_ADJ; ++i) {
        size_t from = index_dist(gen);
        size_t to   = index_dist(gen);
        if (from == to) continue;

        graph.remove_edge(from, to);

        remove_from_list(ledges, from, to);
        if constexpr (!Directed) {
            remove_from_list(ledges, to, from);
        }
    }
}

BOOST_AUTO_TEST_CASE(Test01)
{
    for (size_t i = 0; i < 10; ++i) {
        list<vpair> ledges;
        SparseGraph<size_t, 20, true> graph;
        graph.resize(20);
        BOOST_REQUIRE(graph.size() == 20);

        test_insertion<>(graph, ledges);
        auto observed = to_string(graph);
        auto expected = to_string(ledges);
        boost_test(expected, observed);

        test_removal<>(graph, ledges);
        observed = to_string(graph);
        expected = to_string(ledges);
        boost_test(expected, observed);
    }

    for (size_t i = 0; i < 10; ++i) {
        list<vpair> ledges;
        SparseGraph<size_t, 20, false> graph;
        graph.resize(20);
        BOOST_REQUIRE(graph.size() == 20);

        test_insertion<>(graph, ledges);
        auto observed = to_string(graph);
        auto expected = to_string(ledges);
        boost_test(expected, observed);

        test_removal<>(graph, ledges);
        observed = to_string(graph);
        expected = to_string(ledges);
        boost_test(expected, observed);
    }
}

