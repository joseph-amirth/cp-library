#pragma once

#include "data_structures/tensor.hpp"
#include <initializer_list>

namespace linear_algebra {

template <typename T>
struct matrix : public data_structures::tensor<T, 2> {
    using data_structures::tensor<T, 2>::shape;

    matrix(int m, int n) : data_structures::tensor<T, 2>({m, n}) {}

    matrix(std::initializer_list<std::initializer_list<T>> l) : matrix(l.size(), l.size() ? l.begin()->size() : 0) {
        int i = 0;
        for (auto row : l) {
            int j = 0;
            for (auto cell : row) {
                (*this)[i][j] = cell;
                j += 1;
            }
            i += 1;
        }
    }

    matrix &operator+=(const matrix &other) {
        auto [m, n] = shape;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                (*this)[i][j] += other[i][j];
            }
        }
        return *this;
    }

    matrix &operator-=(const matrix &other) {
        auto [m, n] = shape;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                (*this)[i][j] -= other[i][j];
            }
        }
        return *this;
    }

    matrix &operator*=(const matrix &other) {
        return (*this = *this + other);
    }

    matrix operator+(const matrix &other) const {
        return matrix(*this) += other;
    }

    matrix operator-(const matrix &other) const {
        return matrix(*this) -= other;
    }

    matrix operator*(const matrix &other) const {
        auto [m1, n1] = shape;
        auto [m2, n2] = other.shape;
        assert(n1 == m2);

        matrix result(m1, n2);
        for (int i = 0; i < m1; i++) {
            for (int j = 0; j < n1; j++) {
                for (int kk = 0; kk < n2; kk++) {
                    result[i][kk] += (*this)[i][j] * other[j][kk];
                }
            }
        }
        return result;
    }
};

} // namespace linear_algebra
