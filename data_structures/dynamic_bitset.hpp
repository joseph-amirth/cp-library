#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

namespace data_structures {

struct dynamic_bitset {
    static constexpr int W = 64;

    using word_type = unsigned long long;

    std::size_t n;
    std::vector<word_type> words;

    dynamic_bitset(int n) : n(n), words((n + W - 1) / W) {}

    std::size_t size() const {
        return n;
    }

    bool test(std::size_t i) const {
        return words[i / W] >> (i % W) & 1;
    }

    std::size_t find_first() const {
        for (int i = 0; i < (int)words.size(); i++) {
            if (words[i]) {
                word_type lsb = words[i] & -words[i];
                return i * W + __builtin_ctzll(lsb);
            }
        }
        return size();
    }

    dynamic_bitset &set(std::size_t i) {
        words[i / W] |= 1ull << (i % W);
        return *this;
    }

    dynamic_bitset &reset(std::size_t i) {
        words[i / W] &= ~(1ull << (i % W));
        return *this;
    }

    dynamic_bitset &flip(std::size_t i) {
        words[i / W] ^= 1ull << (i % W);
        return *this;
    }

    dynamic_bitset &operator&=(const dynamic_bitset &other) {
        assert(size() == other.size());
        for (int i = 0; i < (int)words.size(); i++) {
            words[i] &= other.words[i];
        }
        return *this;
    }

    dynamic_bitset &operator|=(const dynamic_bitset &other) {
        assert(size() == other.size());
        for (int i = 0; i < (int)words.size(); i++) {
            words[i] |= other.words[i];
        }
        return *this;
    }

    dynamic_bitset &operator^=(const dynamic_bitset &other) {
        assert(size() == other.size());
        for (int i = 0; i < (int)words.size(); i++) {
            words[i] ^= other.words[i];
        }
        return *this;
    }

    friend dynamic_bitset operator&(const dynamic_bitset &lhs, const dynamic_bitset &rhs) {
        return dynamic_bitset(lhs) &= rhs;
    }

    friend dynamic_bitset operator|(const dynamic_bitset &lhs, const dynamic_bitset &rhs) {
        return dynamic_bitset(lhs) |= rhs;
    }

    friend dynamic_bitset operator^(const dynamic_bitset &lhs, const dynamic_bitset &rhs) {
        return dynamic_bitset(lhs) ^= rhs;
    }
};

} // namespace data_structures
