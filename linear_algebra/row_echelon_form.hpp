#pragma once

#include "matrix.hpp"
#include <utility>

namespace linear_algebra {

template <typename T>
bool row_echelon_form(matrix<T> &a) {
    auto [m, n] = a.shape;

    bool flip = false;
    for (int col = 0, row = 0; col < n; col++) {
        int nonzero = -1;
        for (int i = row; i < m; i++) {
            if (a[i][col] != 0) {
                nonzero = i;
                break;
            }
        }
        if (nonzero == -1) {
            continue;
        }
        if (nonzero != row) {
            flip = !flip;
            for (int j = 0; j < n; j++) {
                std::swap(a[row][j], a[nonzero][j]);
            }
        }
        for (int i = row + 1; i < m; i++) {
            T multiplier = a[i][col] / a[row][col];
            for (int j = col; j < n; j++) {
                a[i][j] -= multiplier * a[row][j];
            }
        }
        row += 1;
    }

    return flip;
}

} // namespace linear_algebra
