#pragma once

#include "matrix.hpp"
#include "row_echelon_form.hpp"

namespace linear_algebra {

template <typename T>
int rank(const matrix<T> &a) {
    auto [m1, n1] = a.shape;
    matrix a_copy = (m1 < n1 ? a.transpose() : a);
    row_echelon_form(a_copy);

    auto [m2, n2] = a_copy.shape;

    int nonzero = 0;
    for (int i = 0; i < m2; i++) {
        for (int j = 0; j < n2; j++) {
            if (a_copy[i][j] != 0) {
                nonzero += 1;
                break;
            }
        }
    }
    return nonzero;
}

} // namespace linear_algebra
