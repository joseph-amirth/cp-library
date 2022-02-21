#pragma once

#include <functional>
#include <cassert>
#include "../tensor/dynamic-tensor.hpp"

template <typename T, int D = 1, typename F = std::function<T(const T&, const T&)>>
struct nd_sparse_table {
    static_assert(D > 0, "Dimension must be positive");

    int n;
    dynamic_tensor<std::conditional_t<D == 1, T, nd_sparse_table<T, D - 1, F>>, 2> mat;
    F f;

    nd_sparse_table() : n(), f() {}

    template <typename U>
    nd_sparse_table(const dynamic_tensor<U, D> &t, F f) : n(t.size()), f(f) {
        assert(n > 0);
        int lg = 32 - __builtin_clz(n);

        mat.resize(lg), mat[0].resize(n);
        for (int i = 0; i < n; i++) {
            if constexpr (D == 1) {
                mat[0][i] = T(t[i]);
            } else {
                mat[0][i] = nd_sparse_table<T, D - 1, F>(t[i], f);
            }
        }

        for (int j = 1; j < lg; j++) {
            mat[j].resize(n - (1 << j) + 1);
            for (int i = 0; i + (1 << j) <= n; i++) {
                if constexpr (D == 1) {
                    mat[j][i] = f(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
                } else {
                    mat[j][i] = nd_sparse_table<T, D - 1, F>(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
                }
            }
        }
    }

    nd_sparse_table(nd_sparse_table<T, D, F> a, const nd_sparse_table<T, D, F> &b) {
        std::swap(*this, a);
        inplace_combine_tensors(this->mat, b.mat, f);
    }

    template <typename...args>
    auto query(int l, int r, const args...tail) {
        assert(0 <= l && l <= r && r < n);
        if constexpr (D == 1) {
            int j = 32 - __builtin_clz(r - l + 1) - 1;
            return f(mat[j][l], mat[j][r + 1 - (1 << j)]);
        } else {
            int j = 32 - __builtin_clz(r - l + 1) - 1;
            return f(mat[j][l].query(tail...), mat[j][r - (1 << j) + 1].query(tail...));
        }
    }
};