#pragma once

#include <array>

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