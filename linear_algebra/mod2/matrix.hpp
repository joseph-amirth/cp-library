#pragma once

#include "data_structures/dynamic_bitset.hpp"

#include <vector>

namespace linear_algebra {

struct matrix_mod2 {
    std::vector<data_structures::dynamic_bitset> data;

    matrix_mod2(int m, int n) : data(m, data_structures::dynamic_bitset(n)) {}

    size_t rows() const {
        return data.size();
    }

    size_t cols() const {
        return data.empty() ? 0 : data.front().size();
    }

    matrix_mod2 operator*(const matrix_mod2 &other) const {
        int m1 = (int)rows(), n1 = (int)cols();
        int m2 = (int)other.rows(), n2 = (int)other.cols();
        assert(n1 == m2);

        matrix_mod2 result(m1, n2);
        for (int i = 0; i < m1; i++) {
            for (int j = 0; j < n1; j++) {
                if (data[i].test(j)) {
                    result.data[i] ^= other.data[j];
                }
            }
        }
        return result;
    }
};

} // namespace linear_algebra
