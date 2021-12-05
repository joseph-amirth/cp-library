#pragma once

#include <string>
#include <vector>
#include <array>

template<char f = 'a', size_t K = 26>
struct aho_corasick {
    struct node {
        bool leaf = false;
        int idx = -1;
        char pch;
        int p;
        int link = -1;
        int exit_link = -1;
        std::array<int, K> next;

        node(int p = -1, char pch = '$') : p(p), pch(pch) {
            next.fill(-1);
        }
    };

    std::vector<node> t;

    aho_corasick() : t(1) {}

    void insert(const std::string &s, int idx = -1) {
        int cur = 0;
        for (char ch: s) {
            if (t[cur].next[ch - f] == -1) {
                t[cur].next[ch - f] = (int) t.size();
                t.emplace_back(cur, ch);
            }
            cur = t[cur].next[ch - f];
        }
        t[cur].idx = idx;
        t[cur].leaf = true;
    }

    bool search(const std::string &s) const {
        int cur = 0;
        for (char ch: s) {
            if (t[cur].next[ch - f] == -1) {
                return false;
            }
            cur = t[cur].next[ch - f];
        }
        return t[cur].leaf;
    }

    void process() {
        std::vector<int> q;
        q.reserve(t.size());
        q.push_back(0);
        for (int qi = 0; qi < int(q.size()); qi++) {
            int x = q[qi];
            if (x == 0 or t[x].p == 0) {
                t[x].link = 0;
            } else {
                t[x].link = t[t[t[x].p].link].next[t[x].pch - f];
                t[x].exit_link = t[t[x].link].leaf ? t[x].link : t[t[x].link].exit_link;
            }

            for (int i = 0; i < K; i++) {
                if (t[x].next[i] == -1) {
                    t[x].next[i] = x == 0 ? 0 : t[t[x].link].next[i];
                } else {
                    q.push_back(t[x].next[i]);
                }
            }
        }
    }

    node &operator[](int i) {
        return t[i];
    }
};