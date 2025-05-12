#pragma once

#include "linear_algebra/reduced_row_echelon_form.hpp"
#include "matrix.hpp"
#include <optional>

namespace linear_algebra {

template <typename T>
std::optional<matrix<T>> inv(const matrix<T> &a) {
    auto [m, n] = a.shape;
    assert(m == n);

    matrix<T> b(n, 2 * n);
    for (int i = 0; i < n; i++) {
        b[i][i + n] = T(1);
        for (int j = 0; j < n; j++) {
            b[i][j] = a[i][j];
        }
    }

    reduced_row_echelon_form(b);

    for (int i = 0; i < n; i++) {
        if (b[i][i] == 0) {
            return std::nullopt;
        }
    }

    matrix<T> result(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = b[i][j + n];
        }
    }
    return result;
}

} // namespace linear_algebra
