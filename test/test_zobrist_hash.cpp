#define BOOST_TEST_MODULE ZOBRIST_HASH_TESTS

#include <boost/test/unit_test.hpp>
#include "zobrist_hash.h"
#include <bitset>

using namespace std;

using ull_t = unsigned long long;
constexpr size_t BIT_COUNT = std::numeric_limits<ull_t>::digits;

bool check_different(size_t ind1, size_t ind2, size_t octet, ull_t hash1, ull_t hash2) {
    const ull_t mask = static_cast<ull_t>(0xFF) << (octet * 8);

    /* BOOST_CHECK_MESSAGE((hash1 & mask) != (hash2 & mask), */
    if ((hash1 & mask) == (hash2 & mask)) {
        cerr << "\nind1=" << ind1 << ", ind2=" << ind2 << ", octet=" << octet
             << "\nhash1 bits:   " << bitset<BIT_COUNT>(hash1)
             << "\nhash2 bits:   " << bitset<BIT_COUNT>(hash2)
             << "\nmask:         " << bitset<BIT_COUNT>(mask)
             << "\nhash1 masked: " << bitset<BIT_COUNT>(hash1 & mask)
             << "\nhash2 masked: " << bitset<BIT_COUNT>(hash2 & mask);
    }
    return (hash1 & mask) == (hash2 & mask);
}

BOOST_AUTO_TEST_CASE(Test01)
{
    constexpr size_t HSIZE = 10;
    ZobristHash<HSIZE, ull_t> zhash;

    size_t collision_count = 0u;
    for (size_t i = 0; i < HSIZE - 1; i++) {
        for (size_t j = i + 1; j < HSIZE; j++) {
            ull_t hash1 = zhash.random_bits(i);
            ull_t hash2 = zhash.random_bits(j);

            for (size_t k = 0; k < BIT_COUNT / 8; k++) {
                if (check_different(i, j, k, hash1, hash2)) { collision_count++; }
            }
        }
    }

    BOOST_REQUIRE_MESSAGE(collision_count <= 3,
                   "\ncollisions_count = " << collision_count);
}

