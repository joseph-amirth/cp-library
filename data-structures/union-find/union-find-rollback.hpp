#pragma once

#include <vector>
#include <numeric>

struct union_find {
    int n, c;
    std::vector<int> p, r, st;

    union_find() : n(), c() {}

    union_find(int n) : n(n), c(n), p(n), r(n, 1) {
        std::iota(p.begin(), p.end(), 0);
    }

    int count() const noexcept {
        return c;
    }

    int find(int x) {
        while (x != p[x]) {
            x = p[x];
        }
        return x;
    }

    bool merge(int x, int y) {
        int l1 = find(x);
        int l2 = find(y);
        if (l1 == l2) {
            st.push_back(-1);
            return false;
        }
        if (r[l1] >= r[l2]) {
            r[l1] += r[l2];
            p[l2] = l1;
            st.push_back(l2);
        } else {
            p[l1] = l2;
            r[l2] += r[l1];
            st.push_back(l1);
        }
        c -= 1;
        return true;
    }

    bool roll_back() {
        int l = st.back();
        st.pop_back();
        if (l == -1) {
            return false;
        }
        r[p[l]] -= r[l];
        p[l] = l;
        c += 1;
        return true;
    }
};