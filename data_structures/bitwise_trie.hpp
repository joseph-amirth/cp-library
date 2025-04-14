#pragma once

#include <array>
#include <bitset>
#include <vector>

template <int N>
struct bitwise_trie {
    struct node {
        int cnt;
        std::array<int, 2> next;

        node() : cnt(0) {
            next.fill(-1);
        }
    };

    std::vector<node> t;

    bitwise_trie() : t(1) {}

    void insert(const std::bitset<N> &b, int freq = 1) {
        int cur = 0;
        t[cur].cnt += freq;
        for (int i = N - 1; i >= 0; i--) {
            if (t[cur].next[b[i]] == -1) {
                t[cur].next[b[i]] = (int)t.size();
                t.emplace_back();
            }
            cur = t[cur].next[b[i]];
            t[cur].cnt += freq;
        }
    }

    void remove(const std::bitset<N> &b, int freq = 1) {
        insert(b, -freq);
    }

    int count(const std::bitset<N> &b) {
        int cur = 0;
        for (int i = N - 1; i >= 0; i--) {
            if (t[cur].next[b[i]] == -1) {
                return 0;
            }
            cur = t[cur].next[b[i]];
        }
        return t[cur].cnt;
    }

    std::bitset<N> xor_minimum(std::bitset<N> b) {
        assert(t[0].cnt > 0);
        int cur = 0;
        for (int i = N - 1; i >= 0; i--) {
            if (t[cur].next[b[i]] != -1 && t[t[cur].next[b[i]]].cnt > 0) {
                cur = t[cur].next[b[i]];
                b.reset(i);
            } else {
                cur = t[cur].next[1 ^ b[i]];
                b.set(i);
            }
        }
        return b;
    }

    std::bitset<N> xor_maximum(const std::bitset<N> &b) {
        return ~xor_minimum(~b);
    }

    node &operator[](int i) {
        return t[i];
    }
};
