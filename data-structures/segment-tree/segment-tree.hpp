#pragma once

#include <vector>
#include <functional>

template<typename T>
struct segment_tree {
    using F = std::function<T(const T&, const T&)>;

    int n;
    std::vector<T> t;
    T e;
    F f;

    segment_tree() : n(), e(), f() {}

    template<typename U>
    segment_tree(const std::vector<U> &v, int n, T e, F &&f): n(n), t(4 * n), e(e), f(f) {
        build(v, 1, 0, n - 1);
    }

    template<typename U>
    void build(const std::vector<U> &v, int i, int l, int r) {
        if (l == r) {
            t[i] = T(v[l]);
        } else {
            int mid = (l + r) >> 1;
            build(v, i << 1, l, mid);
            build(v, i << 1 | 1, mid + 1, r);
            t[i] = f(t[i << 1], t[i << 1 | 1]);
        }
    }

    template<typename U>
    void update(int idx, U val) {
        update_helper(idx, val, 1, 0, n - 1);
    }

    template<typename U>
    void update_helper(int idx, U val, int i, int l, int r) {
        if (l == r) {
            t[i] = T(val);
        } else {
            int mid = (l + r) >> 1;
            if (idx <= mid) {
                update_helper(idx, val, i << 1, l, mid);
            } else {
                update_helper(idx, val, i << 1 | 1, mid + 1, r);
            }
            t[i] = f(t[i << 1], t[i << 1 | 1]);
        }
    }

    T query(int ql, int qr) {
        return query_helper(ql, qr, 1, 0, n - 1);
    }

    T query_helper(int ql, int qr, int i, int l, int r) {
        if (qr < l || r < ql) {
            return e;
        } else if (ql <= l && r <= qr) {
            return t[i];
        } else {
            int mid = (l + r) >> 1;
            T x = query_helper(ql, qr, i << 1, l, mid), y = query_helper(ql, qr, i << 1 | 1, mid + 1, r);
            return f(x, y);
        }
    }
};