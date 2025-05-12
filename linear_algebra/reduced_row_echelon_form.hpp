#pragma once

#include "matrix.hpp"
#include "row_echelon_form.hpp"

namespace linear_algebra {

template <typename T>
bool reduced_row_echelon_form(matrix<T> &a) {
    bool flip = row_echelon_form(a);

    auto [m, n] = a.shape;
    for (int row = m - 1; row >= 0; row--) {
        for (int col = 0; col < n; col++) {
            if (a[row][col] != 0) {
                T div = T(1) / a[row][col];
                for (int j = col; j < n; j++) {
                    a[row][j] *= div;
                }
                for (int i = row - 1; i >= 0; i--) {
                    T multiplier = a[i][col];
                    for (int j = col; j < n; j++) {
                        a[i][j] -= multiplier * a[row][j];
                    }
                }
                break;
            }
        }
    }

    return flip;
}

} // namespace linear_algebra
