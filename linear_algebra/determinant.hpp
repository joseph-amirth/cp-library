#pragma once

#include "matrix.hpp"
#include "row_echelon_form.hpp"

namespace linear_algebra {

template <typename T>
T determinant(const matrix<T> &a) {
    auto [m, n] = a.shape;
    assert(m == n);

    matrix a_copy = a;
    bool flip = row_echelon_form(a_copy);

    T prod = T(1);
    for (int i = 0; i < m; i++) {
        prod *= a_copy[i][i];
    }
    return (flip ? -prod : prod);
}

} // namespace linear_algebra
