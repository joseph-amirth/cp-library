#pragma once

#include <vector>
#include <cassert>

template<typename T>
struct matrix : std::vector<std::vector<T>> {
    int n, m;

    matrix() : n(), m() {}

    matrix(int n, int m, const T val = T()) : n(n), m(m), std::vector<std::vector<T>>(n, std::vector<T>(m, val)) {}

    matrix(std::initializer_list<std::vector<T>> l) : std::vector<std::vector<T>>(l) {
        n = l.size();
        m = l.size() ? l.begin()->size() : 0;
    }

    matrix &operator+=(const matrix &mat) {
        assert(n == mat.n && m == mat.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                (*this)[i][j] += mat[i][j];
            }
        }
        return *this;
    }

    matrix &operator-=(const matrix &mat) {
        assert(n == mat.n && m == mat.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                (*this)[i][j] -= mat[i][j];
            }
        }
        return *this;
    }

    matrix &operator*=(const matrix &mat) {
        assert(m == mat.n);
        matrix res(n, mat.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < mat.m; j++) {
                for (int k = 0; k < m; k++) {
                    res[i][j] += (*this)[i][k] * mat[k][j];
                }
            }
        }
        this->swap(res);
        return *this;
    }

    matrix &operator*=(const T &val) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                (*this)[i][j] *= val;
            }
        }
        return *this;
    }

    static matrix identity(int n) {
        matrix I(n, n);
        for (int i = 0; i < n; i++) {
            I[i][i] = 1;
        }
        return I;
    }

    matrix pow(long long k) {
        matrix ans = identity(n), x = *this;
        for (; k; k /= 2) {
            if (k & 1) ans *= x;
            x *= x;
        }
        return ans;
    }

    friend matrix operator+(matrix a, const matrix &b) {
        a += b;
        return a;
    }

    friend matrix operator-(matrix a, const matrix &b) {
        a -= b;
        return a;
    }

    friend matrix operator*(matrix a, const matrix &b) {
        a *= b;
        return a;
    }

    friend matrix operator*(matrix a, const T &val) {
        a *= val;
        return a;
    }

    friend matrix operator*(const T &val, matrix a) {
        a *= val;
        return a;
    }
};
