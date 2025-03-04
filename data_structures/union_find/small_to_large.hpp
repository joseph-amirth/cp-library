#pragma once

#include <vector>

namespace data_structures {

struct union_find {
    int n, c;
    std::vector<int> p;
    std::vector<std::vector<int>> v;

    union_find() : n(), c() {}

    union_find(int n) : n(n), c(n), p(n), v(n) {
        for (int i = 0; i < n; i++) {
            p[i] = i, v[i] = {i};
        }
    }

    int count() const noexcept {
        return c;
    }

    int find(int x) {
        return p[x];
    }

    bool merge(int x, int y) {
        int s1 = find(x);
        int s2 = find(y);
        if (s1 == s2) {
            return false;
        }
        if (v[s2].size() > v[s1].size()) {
            std::swap(s1, s2);
        }
        for (int i : v[s2]) {
            v[s1].push_back(i);
            p[i] = s1;
        }
        v[s2].clear();
        c -= 1;
        return true;
    }
};

} // namespace data_structures
