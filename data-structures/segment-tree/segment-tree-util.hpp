#pragma once

#include <cassert>

namespace segment_tree_ops {
    template<typename F>
    void build_tree(int i, int l, int r, F &&init) {
        if (l == r) {
            init(i);
        } else {
            int m = (l + r) >> 1;
            int j = i + ((m - l + 1) << 1);
            build_tree(i + 1, l, m, init);
            build_tree(j, m + 1, r, init);
        }
    }

    int visit_point(int p, int i, int l, int r) {
        assert(l <= p && p <= r);
        while (l != r) {
            int m = (l + r) >> 1;
            int j = i + ((m - l + 1) << 1);
            if (p <= m) {
                i += 1, r = m;
            } else {
                i = j, l = m + 1;
            }
        }
        return i;
    }

    template<typename F>
    void visit_range(int ql, int qr, int i, int l, int r, F &&f) {
        if (qr < l || r < ql) {

        } else if (ql <= l && r <= qr) {
            f(i);
        } else {
            int m = (l + r) >> 1;
            int j = i + ((m - l + 1) << 1);
            visit_range(ql, qr, i + 1, l, m, f);
            visit_range(ql, qr, j, m + 1, r, f);
        }
    }

    template<typename F>
    void visit_inverted_range(int ql, int qr, int i, int l, int r, F &&f) {
        if (qr < l || r < ql) {
            f(i);
        } else if (ql <= l && r <= qr) {

        } else {
            int m = (l + r) >> 1;
            int j = i + ((m - l + 1) << 1);
            visit_range(ql, qr, i + 1, l, m, f);
            visit_range(ql, qr, j, m + 1, r, f);
        }
    }

    template<typename P>
    int find_first_of(int ql, int qr, int i, int l, int r, P &&p) {
        if (qr < l || r < ql) {
            return r + 1;
        } else if (l == r) {
            return l;
        } else {
            int m = (l + r) >> 1;
            int j = i + ((m - l + 1) << 1);
            int k = find_first_of(ql, qr, i + 1, l, m, p);
            if (k == m + 1) {
                return find_first_of(ql, qr, j, m + 1, r, p);
            } else {
                return k;
            }
        }
    }

    template<typename P>
    int find_last_of(int ql, int qr, int i, int l, int r, P &&p) {
        if (qr < l || r < ql) {
            return r + 1;
        } else if (l == r) {
            return l;
        } else {
            int m = (l + r) >> 1;
            int j = i + ((m - l + 1) << 1);
            int k = find_last_of(ql, qr, j, m + 1, r, p);
            if (k == r + 1) {
                return find_last_of(ql, qr, i + 1, l, m, p);
            } else {
                return k;
            }
        }
    }
}