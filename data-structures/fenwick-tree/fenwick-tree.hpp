#pragma once

#include <vector>
#include <functional>

template<typename T, typename F = std::function<T(const T&, const T&)>>
struct fenwick_tree {
    int n;
    std::vector<T> bit;
    F f;

    fenwick_tree() : n(), f() {}

    fenwick_tree(int n, F &&f = std::plus<T>()) : n(n), bit(n + 1, 0), f(f) {}

    void add(int i, T val) {
        for (++i; i <= n; i += i & -i) {
            bit[i] = f(bit[i], val);
        }
    }

    T query(int i) {
        T ans = 0;
        for (++i; i; i -= i & -i) {
            ans = f(ans, bit[i]);
        }
        return ans;
    }
};