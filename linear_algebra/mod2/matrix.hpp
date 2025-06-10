#pragma once

#include "data_structures/dynamic_bitset.hpp"

#include <vector>

namespace linear_algebra::mod2 {

struct matrix {
    using row_type = data_structures::dynamic_bitset;

    std::vector<row_type> data;

    matrix(int m, int n) : data(m, row_type(n)) {}

    size_t rows() const {
        return data.size();
    }

    size_t cols() const {
        return data.empty() ? 0 : data.front().size();
    }

    matrix transpose() const {
        int m = (int)rows(), n = (int)cols();
        matrix result(n, m);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (data[i].test(j)) {
                    result[j].set(i);
                }
            }
        }
        return result;
    }

    matrix operator*(const matrix &other) const {
        int m1 = (int)rows(), n1 = (int)cols();
        int m2 = (int)other.rows(), n2 = (int)other.cols();
        assert(n1 == m2);

        matrix result(m1, n2);
        for (int i = 0; i < m1; i++) {
            for (int j = 0; j < n1; j++) {
                if (data[i].test(j)) {
                    result.data[i] ^= other.data[j];
                }
            }
        }
        return result;
    }

    row_type &operator[](int i) {
        return data[i];
    }

    const row_type &operator[](int i) const {
        return data[i];
    }
};

} // namespace linear_algebra::mod2
