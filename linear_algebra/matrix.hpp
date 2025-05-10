#pragma once

#include "data_structures/tensor.hpp"
#include <initializer_list>

namespace linear_algebra {

template <typename T>
struct matrix : public data_structures::tensor<T, 2> {
    using data_structures::tensor<T, 2>::size;

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

    int rows() const {
        return size();
    }

    int cols() const {
        return size() ? (*this)[0].size() : 0;
    }

    matrix &operator+=(const matrix &other) {
        int m = rows(), n = cols();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                (*this)[i][j] += other[i][j];
            }
        }
        return *this;
    }

    matrix &operator-=(const matrix &other) {
        int m = rows(), n = cols();
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
        int m = rows(), n = cols(), k = other.cols();
        assert(n == other.rows());

        matrix result(m, k);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                for (int kk = 0; kk < k; kk++) {
                    result[i][kk] += (*this)[i][j] * other[j][kk];
                }
            }
        }
        return result;
    }
};

} // namespace linear_algebra
