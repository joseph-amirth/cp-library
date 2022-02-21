#pragma once

#include <vector>
#include <array>

namespace vector_variants {
    template<typename T, typename shift_t = int>
    struct shifted_vector : std::vector<T> {
        shift_t shift_value = 0;

        template<typename...args>
        shifted_vector(shift_t shift_value = 0, args...arg) : shift_value(shift_value), std::vector<T>(arg...) {}

        T &operator[](int x) {
            return std::vector<T>::operator[](x + shift_value);
        }

        const T &operator[](int x) const {
            return std::vector<T>::operator[](x + shift_value);
        }
    };

    template<typename T, typename shift_t>
    shifted_vector<T, shift_t> range_vector(shift_t l, shift_t r) {
        return shifted_vector<T, shift_t>(-l, r - l + 1);
    }
}