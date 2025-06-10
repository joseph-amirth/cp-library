#pragma once

#include "matrix.hpp"
#include "row_echelon_form.hpp"

namespace linear_algebra::mod2 {

bool determinant(const matrix &a) {
    int m = (int)a.rows(), n = (int)a.cols();
    assert(m == n);

    matrix a_copy = a;
    bool flip = row_echelon_form(a_copy);

    bool prod = true;
    for (int i = 0; i < m; i++) {
        prod = prod && a_copy[i].test(i);
    }
    return (flip ? -prod : prod);
}

} // namespace linear_algebra::mod2
