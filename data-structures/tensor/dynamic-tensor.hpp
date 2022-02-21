#pragma

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