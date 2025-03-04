#pragma once

#include <numeric>
#include <vector>

namespace data_structures {

struct union_find {
    int n, c;
    std::vector<int> p, r;

    union_find() : n(), c() {}

    union_find(int n) : n(n), c(n), p(n), r(n, 1) {
        std::iota(p.begin(), p.end(), 0);
    }

    int count() const noexcept {
        return c;
    }

    int find(int x) {
        return x == p[x] ? x : p[x] = find(p[x]);
    }

    bool merge(int x, int y) {
        int l1 = find(x);
        int l2 = find(y);
        if (l1 == l2) {
            return false;
        }
        if (r[l1] >= r[l2]) {
            r[l1] += r[l2];
            p[l2] = l1;
        } else {
            p[l1] = l2;
            r[l2] += r[l1];
        }
        c -= 1;
        return true;
    }
};

} // namespace data_structures
