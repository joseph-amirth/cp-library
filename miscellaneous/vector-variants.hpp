#pragma once

#include <vector>
#include <array>

namespace vector_variants {
    template<typename T, int N, int...args>
    struct static_tensor : std::array<static_tensor<T, args...>, N> {
        static_tensor() : std::array<static_tensor<T, args...>, N>() {}

        void fill(const T &val) {
            for (int i = 0; i < N; i++) {
                (*this)[i].fill(val);
            }
        }
    };

    template<typename T, int N>
    struct static_tensor<T, N> : std::array<T, N> {
        static_tensor() : std::array<T, N>() {}
    };

    template<typename T, int D>
    struct dynamic_tensor : std::vector<dynamic_tensor<T, D - 1>> {
        dynamic_tensor() : std::vector<dynamic_tensor<T, D - 1>>() {}

        template<typename...args>
        dynamic_tensor(int n, args...A) : std::vector<dynamic_tensor<T, D - 1>>(n, dynamic_tensor<T, D - 1>(A...)) {}

        dynamic_tensor(const std::initializer_list<dynamic_tensor<T, D - 1>> &list) : std::vector<dynamic_tensor<T,
                D - 1>>(list) {}

        void fill(const T &val) {
            for (auto it = this->begin(); it != this->end(); it++) {
                it->fill(val);
            }
        }
    };

    template<typename T>
    struct dynamic_tensor<T, 1> : std::vector<T> {
        dynamic_tensor() : std::vector<T>() {}

        dynamic_tensor(int n, const T &val = T()) : std::vector<T>(n, val) {}

        dynamic_tensor(const std::initializer_list<T> &list) : std::vector<T>(list) {}

        void fill(const T &val = T()) {
            std::fill(this->begin(), this->end(), val);
        }
    };

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