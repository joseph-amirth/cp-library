#pragma once

#include "matrix.hpp"

namespace linear_algebra::mod2 {

bool row_echelon_form(matrix &a) {
    int m = (int)a.rows(), n = (int)a.cols();

    bool flip = false;
    for (int col = 0, row = 0; col < n; col++) {
        int nonzero = -1;
        for (int i = row; i < m; i++) {
            if (a[i].test(col)) {
                nonzero = i;
                break;
            }
        }
        if (nonzero == -1) {
            continue;
        }
        if (nonzero != row) {
            flip = !flip;
            a[row].swap(a[nonzero]);
        }
        for (int i = row + 1; i < m; i++) {
            if (a[i].test(col)) {
                a[i] ^= a[row];
            }
        }
        row += 1;
    }

    return flip;
}

} // namespace linear_algebra::mod2
