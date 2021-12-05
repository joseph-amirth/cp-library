#pragma once

#include "../miscellaneous/vector-variants.hpp"
#include <string>

using vector_variants::shifted_vector;

template<char f = 'a', int K = 26>
struct eertree {
    struct node {
        int len;
        int link;
        std::array<int, K> next;

        node(int len, int link) : len(len), link(link) {
            next.fill(-1);
        }
    };

    int final_len;
    std::string final;

    shifted_vector<node> t;

    eertree() {
        final_len = 0;
        t = shifted_vector<node>(1);
        t.emplace_back(-1, -1);
        t.emplace_back(0, -1);
    }

    eertree(const std::string &s) : eertree() {
        add(s);
    }

    eertree(const std::vector<std::string> &ss) : eertree() {
        for (const auto &s: ss) {
            add(s);
        }
    }

    void add(const std::string &s) {
        for (auto ch: s) {
            add(ch);
        }
    }

    void add(char ch) {
        static int max_suf = 0;
        int q = max_suf;
        while (q != -1 && (final_len < t[q].len + 1 || final[final_len - t[q].len - 1] != ch)) {
            q = t[q].link;
        }
        if (t[q].next[ch - f] == -1) {
            t[q].next[ch - f] = (int) t.size() - 1;
            t.emplace_back(t[q].len + 2, -1);
            max_suf = t[q].next[ch - f];
            if (q != -1) {
                q = t[q].link;
                while (q != -1 && (final_len < t[q].len + 1 || final[final_len - t[q].len - 1] != ch)) {
                    q = t[q].link;
                }
                t.back().link = t[q].next[ch - f];
            } else {
                t.back().link = 0;
            }
        }
        final_len += 1;
        final += ch;
    }

    node &operator[](int q) {
        return t[q];
    }
};