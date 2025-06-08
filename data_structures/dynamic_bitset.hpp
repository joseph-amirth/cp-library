#pragma once

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

namespace data_structures {

struct dynamic_bitset {
    static constexpr int W = 64;

    using word_type = unsigned long long;

    std::size_t n;
    std::vector<word_type> words;

    dynamic_bitset(int n) : n(n), words((n + W - 1) / W) {}

    static dynamic_bitset from_bitstring(const std::string &s) {
        int sz = (int)s.size();
        dynamic_bitset bitset(sz);
        for (int i = 0; i < sz; i++) {
            word_type b = s[i] - '0';
            bitset.words[i / W] |= b << (i % W);
        }
        return bitset;
    }

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

    friend std::ostream &operator<<(std::ostream &os, const dynamic_bitset &bitset) {
        for (int i = 0; i < (int)bitset.n; i++) {
            os << bitset.test(i);
        }
        return os;
    }

    friend std::istream &operator>>(std::istream &is, dynamic_bitset &bitset) {
        std::string s;
        is >> s;
        bitset = dynamic_bitset::from_bitstring(s);
        return is;
    }
};

} // namespace data_structures
