#pragma once

#include <vector>

template<typename T, int D>
struct dynamic_tensor : std::vector<dynamic_tensor<T, D - 1>> {
    dynamic_tensor() : std::vector<dynamic_tensor<T, D - 1>>() {}

    template<typename...args>
    dynamic_tensor(int n, args...A) : std::vector<dynamic_tensor<T, D - 1>>(n, dynamic_tensor<T, D - 1>(A...)) {}

    dynamic_tensor(const std::initializer_list<dynamic_tensor<T, D - 1>> &list) : std::vector<dynamic_tensor<T, D - 1>>(list) {}

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

template <typename T, int D, typename F>
dynamic_tensor<T, D> combine_tensors(dynamic_tensor<T, D> a, const dynamic_tensor<T, D> &b, const F &f) {
    if constexpr (D == 1) {
        for (int i = 0; i < (int) a.size(); i++) {
            a[i] = f(a[i], b[i]);
        }
    } else {
        for (int i = 0; i < (int) a.size(); i++) {
            a[i] = combine_tensors(a[i], b[i], f);
        }
    }
    return a;
}

template <typename T, int D, typename F>
dynamic_tensor<T, D>& inplace_combine_tensors(dynamic_tensor<T, D> &a, const dynamic_tensor<T, D> &b, const F &f) {
    if constexpr (D == 1) {
        for (int i = 0; i < (int) a.size(); i++) {
            a[i] = f(a[i], b[i]);
        }
    } else {
        for (int i = 0; i < (int) a.size(); i++) {
            inplace_combine_tensors(a[i], b[i], f);
        }
    }
    return a;
}
