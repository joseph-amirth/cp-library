#pragma once

#include <vector>
#include <functional>

template<typename T>
struct fenwick_tree_2d {
    using F = std::function<T(const T&, const T&)>;

    int n, m;
    std::vector<std::vector<int>> bit;
    F f;

    fenwick_tree_2d() : n(), m(), f() {}

    fenwick_tree_2d(int n, int m, F &&f = std::plus<T>()) : n(n), m(m), bit(n + 1, std::vector<int>(m + 1)), f(f) {}

    void add(int i, int j, T val) {
        for (; i <= n; i += i & -i) {
            for (; j <= m; j += i & -i) {
                bit[i][j] = f(bit[i][j], val);
            }
        }
    }

    T query(int i, int j) {
        T ans = T();
        for (; i; i -= i & -i) {
            for (; j; j -= j & -j) {
                ans = f(ans, bit[i][j]);
            }
        }
        return ans;
    }
};