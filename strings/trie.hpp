#pragma once

#include "concepts.hpp"

#include <array>
#include <string>
#include <vector>

namespace strings {

template <CharSet CS>
struct trie {
    using char_set = CS;
    using char_type = typename char_set::char_type;

    struct node {
        int cnt;
        std::array<int, char_set::size()> next;

        node() : cnt(0) {
            next.fill(-1);
        }
    };

    std::vector<node> t;

    trie() : t(1) {}

    void insert(const std::string &s) {
        int cur = 0;
        for (char ch : s) {
            if (t[cur].next[char_set::index_of(ch)] == -1) {
                t[cur].next[char_set::index_of(ch)] = (int)t.size();
                t.emplace_back();
            }
            cur = t[cur].next[char_set::index_of(ch)];
        }
        t[cur].cnt += 1;
    }

    int count(const std::string &s) {
        int cur = 0;
        for (char ch : s) {
            if (t[cur].next[char_set::index_of(ch)] == -1) {
                return false;
            }
            cur = t[cur].next[char_set::index_of(ch)];
        }
        return t[cur].cnt;
    }

    node &operator[](int i) {
        return t[i];
    }
};

} // namespace strings
