#ifndef ZOBRIST_HASH_H
#define ZOBRIST_HASH_H

#include <array>
#include <random>
#include <cassert>
/* #include <iostream> */
/* #include <bitset> */

template <size_t SIZE>
class ZobristHash {
    std::array<unsigned long long, SIZE> random_bits = { 0 };

public:
    ZobristHash() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution distr(0.5);

        for (auto & rb: random_bits) {
            for (uint i = 0; i < SIZE; i++) {
                rb |= distr(gen) << i;
            }
            /* std::cout << std::bitset<SIZE>{rb} << std::endl; */
        }
    }

    template <typename T = size_t>
    unsigned long long hash(const T index) const {
        /* assert(index < SIZE); */
        return random_bits[index];
    }

    template <typename T = size_t, size_t ASize>
    unsigned long long hash(const std::array<T, ASize> & indexes,
                            const size_t alength = ASize) const {
        auto result = 0ull;
        for (size_t i = 0; i < alength; ++i) {
            /* assert(index < SIZE); */
            result ^= random_bits[static_cast<size_t>(indexes[i])];
        }
        return result;
    }
};

#endif
