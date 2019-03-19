#ifndef ZOBRIST_HASH_H
#define ZOBRIST_HASH_H

#include <array>
#include <random>
#include <cassert>
#include <limits>
/* #include <iostream> */
/* #include <bitset> */

template <size_t SIZE, typename HASH_TYPE>
class ZobristHash {
    std::array<HASH_TYPE, SIZE> random_bits = { 0 };
    static constexpr size_t BIT_COUNT = std::numeric_limits<HASH_TYPE>::digits;

public:
    ZobristHash() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution distr(0.5);

        for (auto & rb: random_bits) {
            for (size_t i = 0; i < BIT_COUNT; i++) {
                rb |= static_cast<HASH_TYPE>(distr(gen)) << i;
            }
            /* std::cout << std::bitset<BIT_COUNT>{rb} << std::endl; */
        }
    }

    template <typename T = size_t>
    HASH_TYPE hash(T index) const {
        assert(0 <= index && index < SIZE);

        return random_bits[index];
    }

    template <typename Iter>
    HASH_TYPE hash(Iter first, Iter last) const {
        assert(first != last);

        HASH_TYPE result{ 0u };
        for (auto it = first; it != last; ++it) {
            assert(0 <= *it  && *it < SIZE);
            result ^= random_bits[static_cast<size_t>(*it)];
        }
        return result;
    }

    // template <typename T = size_t, size_t ASize>
    // HASH_TYPE hash(const std::array<T, ASize> & indexes,
    //                const size_t alength = ASize) const {
    //     HASH_TYPE result{ 0u };
    //
    //     for (size_t i = 0; i < alength; ++i) {
    //         assert(0 <= index && index < SIZE);
    //         result ^= random_bits[static_cast<size_t>(indexes[i])];
    //     }
    //     return result;
    // }
};

#endif
