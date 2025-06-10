#pragma once

#include "matrix.hpp"
#include "row_echelon_form.hpp"

namespace linear_algebra::mod2 {

int rank(const matrix &a) {
    int m1 = (int)a.rows(), n1 = (int)a.cols();
    matrix a_copy = (m1 < n1 ? a.transpose() : a);
    row_echelon_form(a_copy);

    int m2 = (int)a_copy.rows();

    int nonzero = 0;
    for (int i = 0; i < m2; i++) {
        nonzero += a_copy[i].any();
    }
    return nonzero;
}

} // namespace linear_algebra::mod2
