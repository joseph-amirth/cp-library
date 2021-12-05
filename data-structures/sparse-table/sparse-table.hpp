#pragma once

#include <vector>
#include <functional>
#include <cassert>

template<typename T, typename F = std::function<T(const T&, const T&)>>
struct sparse_table {
    int n;
    std::vector<std::vector<T>> mat;
    F f;

    sparse_table() : n(), f() {}

    sparse_table(std::vector<T> &&v, F &&f) : n(v.size()), f(f) {
        assert(n > 0);
        int lg = 32 - __builtin_clz(n);
        mat.resize(lg), mat[0] = v;
        for (int j = 1; j < mat.size(); j++) {
            mat[j].resize(n - (1 << j) + 1);
            for (int i = 0; i + (1 << j) <= n; i++) {
                mat[j][i] = f(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    T query(int l, int r) {
        assert(0 <= l && l <= r && r < n);
        int j = 32 - __builtin_clz(r - l + 1) - 1;
        return f(mat[j][l], mat[j][r + 1 - (1 << j)]);
    }
};