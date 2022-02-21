#pragma once

#include <vector>
#include <array>
#include <algorithm>
#include <cassert>

template <typename T>
struct flat_tensor_view {
    int D, total_sz;
    int *shape, *strides;

    T *data;

    flat_tensor_view() : D(), total_sz(), shape(), strides(), data() { }

    flat_tensor_view(int D, int total_sz, int *shape, int *strides, T *data) {
        this->D = D;
        this->total_sz = total_sz;
        this->shape = shape;
        this->strides = strides;
        this->data = data;
    }

    flat_tensor_view<T> get_view(const std::initializer_list<int> &index = {}) {
        int offset = 0, i = 0, view_sz = total_sz;
        for (auto ind : index) {
            offset += ind * strides[i];
            view_sz /= shape[i];
            i += 1;
        }
        return flat_tensor_view<T>(D - i, view_sz, shape + i, strides + i, data + offset);
    }

    flat_tensor_view<T> operator[](const int index) {
        return flat_tensor_view<T>(D - 1, *strides, shape + 1, strides + 1, data + index * (*strides));
    }

    const flat_tensor_view<T> operator[](const int index) const {
        return flat_tensor_view<T>(D - 1, *strides, shape + 1, strides + 1, data + index * (*strides));
    }

    T &operator[](const std::initializer_list<int> &index) {
        int offset = 0, i = 0;
        for (auto ind : index) {
            offset += ind * strides[i];
        }
        return data[offset];
    }

    const T &operator[](const std::initializer_list<int> &index) const {
        int offset = 0, i = 0;
        for (auto ind : index) {
            offset += ind * strides[i];
        }
        return data[offset];
    }
};

template <typename T, int D>
struct flat_tensor {
    int total_sz;
    std::array<int, D> shape, strides;

    std::vector<T> data;

    void reshape(const std::array<int, D> &new_shape) {
        assert(std::all_of(new_shape.begin(), new_shape.end(), [](int x) { return x > 0; }));
        strides[0] = 1;
        for (int i = 1; i < D; i++) {
            strides[i] = strides[i - 1] * new_shape[D - i];
        }
        std::reverse(strides.begin(), strides.end());
        total_sz = strides.front() * new_shape.front();
        shape = new_shape;
    }

    flat_tensor() : total_sz(), shape(), strides() {}

    flat_tensor(const std::array<int, D> &shape, T item = T()) : flat_tensor() {
        reshape(shape);
        data.resize(total_sz, item);
    }

    template <typename iterator_t>
    flat_tensor(const std::array<int, D> &shape, iterator_t first, iterator_t last) {
        reshape(shape);
        data.assign(first, last);
        assert(data.size() == total_sz);
    }

    flat_tensor_view<T> get_view(const std::initializer_list<int> &index = {}) const {
        assert(index.size() <= D);
        int offset = 0, i = 0, view_sz = total_sz;
        for (auto ind : index) {
            offset += ind * strides[i];
            view_sz /= shape[i];
            i += 1;
        }
        return flat_tensor_view<T>(D - i, view_sz, shape.data() + i, strides.data() + i, data.data() + offset);
    }

    flat_tensor_view<T> operator[](const int index) {
        return flat_tensor_view<T>(D - 1, strides.front(), shape.data() + 1, strides.data() + 1, data.data() + index * strides.front());
    }

    const flat_tensor_view<T> operator[](const int index) const {
        return flat_tensor_view<T>(D - 1, strides.front(), shape.data() + 1, strides.data() + 1, data.data() + index * strides.front());
    }

    T &operator[](const std::initializer_list<int> &index) {
        int offset = 0, i = 0;
        for (auto ind : index) {
            offset += ind * strides[i];
            i += 1;
        }
        return data[offset];
    }

    const T &operator[](const std::initializer_list<int> &index) const {
        int offset = 0, i = 0;
        for (auto ind : index) {
            offset += ind * strides[i];
            i += 1;
        }
        return data[offset];
    }
};