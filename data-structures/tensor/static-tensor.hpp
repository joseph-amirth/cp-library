#pragma once

#include <array>

template <typename T, int N, int...Ns>
struct static_tensor : std::array<static_tensor<T, Ns...>, N> {
    static_tensor() : std::array<static_tensor<T, Ns...>, N>() {}

    void fill(const T &val) {
        for (int i = 0; i < N; i++) {
            (*this)[i].fill(val);
        }
    }
};

template <typename T, int N>
struct static_tensor<T, N> : std::array<T, N> {
    template <typename...Args>
    static_tensor(Args &&...args) : std::array<T, N>(std::forward<Args>(args)...) {}
};
