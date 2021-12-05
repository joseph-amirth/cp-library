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
    segment_tree(const std::vector<U> &v, int n, T e, F &&f): n(n), t(2 * n), e(e), f(f) {
        std::copy(v.begin(), v.end(), t.begin() + n);
        for (int i = n - 1; i > 0; i--) {
            t[i] = f(t[i << 1], t[i << 1 | 1]);
        }
    }

    template<typename U>
    void update(int i, const U &val) {
        for (t[i += n] = T(val); i > 1; i >>= 1) {
            t[i >> 1] = f(t[i], t[i ^ 1]);
        }
    }

    T query(int l, int r) {
        T ansl = e, ansr = e;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ansl = f(ansl, t[l++]);
            if (r & 1) ansr = f(t[--r], ansr);
        }
        return f(ansl, ansr);
    }
};