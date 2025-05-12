#pragma once

#include "data_structures/tensor.hpp"
#include <cstdint>
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

    static matrix eye(int n) {
        matrix result(n, n);
        for (int i = 0; i < n; i++) {
            result[i][i] = T(1);
        }
        return result;
    }

    matrix transpose() const {
        auto [m, n] = shape;
        matrix result(n, m);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result[j][i] = (*this)[i][j];
            }
        }
        return result;
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
        return (*this = *this * other);
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
                for (int k = 0; k < n2; k++) {
                    result[i][k] += (*this)[i][j] * other[j][k];
                }
            }
        }
        return result;
    }

    matrix pow(std::uint64_t k) const {
        auto [m, n] = shape;
        assert(m == n);

        matrix ans = eye(m), x(*this);
        for (; k > 0; k /= 2) {
            if (k & 1)
                ans *= x;
            x *= x;
        }
        return ans;
    }
};

} // namespace linear_algebra
