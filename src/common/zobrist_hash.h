// Zobrist hashing implementation.
// SIZE      - is the number of random bitstrings,
// HASH_TYPE - is a type that determines the length of a bitstring.

#ifndef ZOBRIST_HASH_H
#define ZOBRIST_HASH_H

#include <array>
#include <random>
#include <cassert>
#include <limits>

template <size_t SIZE, typename HASH_TYPE = unsigned long long>
class ZobristHash {
    std::array<HASH_TYPE, SIZE> _random_bits = { 0 };
    static constexpr size_t BIT_COUNT = std::numeric_limits<HASH_TYPE>::digits;

public:
    ZobristHash() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution distr(0.5);

        for (auto & rb: _random_bits) {
            for (size_t i = 0; i < BIT_COUNT; i++) {
                rb |= static_cast<HASH_TYPE>(distr(gen)) << i;
            }
        }
    }

    HASH_TYPE random_bits(size_t index) const {
        assert(index < _random_bits.size());

        return _random_bits[index];
    }

    template <typename T = size_t>
    HASH_TYPE hash(T index) const {
        assert(0 <= index && index < SIZE);

        return _random_bits[index];
    }

    template <typename Iter>
    HASH_TYPE hash(Iter first, Iter last) const {
        assert(first != last);

        HASH_TYPE result{ 0u };
        for (auto it = first; it != last; ++it) {
            assert(0 <= *it && *it < SIZE);

            result ^= _random_bits[static_cast<size_t>(*it)];
        }
        return result;
    }
};

#endif
