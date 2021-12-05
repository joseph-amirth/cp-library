#pragma once

#include <functional>
#include <cassert>
#include "../../miscellaneous/vector-variants.hpp"

using vector_variants::dynamic_tensor;

template<typename T, typename F = std::function<T(const T&, const T&)>>
struct sparse_table_2d {
    int n, m;
    dynamic_tensor<T, 4> mat;
    F f;

    sparse_table_2d() : n(), m(), f() {}

    template<typename U>
    sparse_table_2d(const U &arr, int n, int m, F &&f): n(n), m(m), f(f) {
        assert(n > 0 && m > 0);

        int lgn = 32 - __builtin_clz(n), lgm = 32 - __builtin_clz(m);
        mat.assign(lgn, lgm);

        mat[0][0].assign(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                mat[0][0][i][j] = T(arr[i][j]);
            }
        }

        for (int k = 1; k < lgn; k++) {
            mat[k][0].assign(n - (1 << k) + 1, m);
            for (int i = 0; i + (1 << k) <= n; i++) {
                for (int j = 0; j < m; j++) {
                    mat[k][0][i][j] = f(mat[k - 1][0][i][j], mat[k - 1][0][i + (1 << (k - 1))][j]);
                }
            }
        }

        for (int k = 0; k < lgn; k++) {
            for (int l = 1; l < lgm; l++) {
                mat[k][l].assign(n - (1 << k) + 1, m - (1 << l) + 1);
                for (int i = 0; i + (1 << k) <= n; i++) {
                    for (int j = 0; j + (1 << l) <= m; j++) {
                        mat[k][l][i][j] = f(mat[k][l - 1][i][j], mat[k][l - 1][i][j + (1 << (l - 1))]);
                    }
                }
            }
        }
    }

    T query(int x1, int y1, int x2, int y2) {
        assert(0 <= x1 && x1 <= x2 && x2 < n);
        assert(0 <= y1 && y1 <= y2 && y2 < m);
        int k = 32 - __builtin_clz(x2 - x1 + 1) - 1, l = 32 - __builtin_clz(y2 - y1 + 1) - 1;
        T res1 = f(mat[k][l][x1][y1], mat[k][l][x2 + 1 - (1 << k)][y1]);
        T res2 = f(mat[k][l][x1][y2 + 1 - (1 << l)], mat[k][l][x2 + 1 - (1 << k)][y2 + 1 - (1 << l)]);
        return f(res1, res2);
    }
};