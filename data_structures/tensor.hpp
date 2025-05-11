#pragma once

#include <array>
#include <cassert>
#include <vector>

namespace data_structures {

template <typename T, int D>
class tensor_view {
    static_assert(0 < D, "Dimension should be positive");

    template <typename T1, int D1>
    friend class tensor_view;

    template <typename T1, int D1>
    friend class tensor;

  public:
    int size() const {
        return shape[0];
    }

    T &operator[](int i)
        requires(D == 1)
    {
        assert(0 <= i && i < shape[0]);
        return data[i];
    }

    const T &operator[](int i) const
        requires(D == 1)
    {
        assert(0 <= i && i < shape[0]);
        return data[i];
    }

    tensor_view<T, D - 1> operator[](int i)
        requires(D > 1)
    {
        assert(0 <= i && i < shape[0]);
        int offset = strides[0] * i;
        return tensor_view<T, D - 1>(&shape[1], &strides[1], &data[offset]);
    }

    const tensor_view<T, D - 1> operator[](int i) const
        requires(D > 1)
    {
        assert(0 <= i && i < shape[0]);
        int offset = strides[0] * i;
        return tensor_view<T, D - 1>(&shape[1], &strides[1], &data[offset]);
    }

  private:
    const int *shape, *strides;
    T *data;

    tensor_view(const int *shape, const int *strides, T *data) : shape(shape), strides(strides), data(data) {}
};

template <typename T, int D>
class tensor {
    static_assert(0 < D, "Dimension should be positive");

  public:
    tensor(const std::array<int, D> &shape, const T &val = T()) : shape(shape), strides(get_strides(shape)) {
        int prod = shape.front() * strides.front();
        data.assign(prod, val);
    }

    int size() const {
        return shape[0];
    }

    T &operator[](int i)
        requires(D == 1)
    {
        assert(0 <= i && i < shape[0]);
        return data[i];
    }

    const T &operator[](int i) const
        requires(D == 1)
    {
        assert(0 <= i && i < shape[0]);
        return data[i];
    }

    tensor_view<T, D - 1> operator[](int i)
        requires(D > 1)
    {
        assert(0 <= i && i < shape[0]);
        int offset = strides[0] * i;
        return tensor_view<T, D - 1>(&shape[1], &strides[1], &data[offset]);
    }

    const tensor_view<T, D - 1> operator[](int i) const
        requires(D > 1)
    {
        assert(0 <= i && i < shape[0]);
        int offset = strides[0] * i;
        return tensor_view<T, D - 1>(&shape[1], &strides[1], const_cast<T *>(&data[offset]));
    }

    const std::array<int, D> shape;

  private:
    static std::array<int, D> get_strides(const std::array<int, D> &shape) {
        std::array<int, D> strides;
        for (int i = D - 1, prod = 1; i >= 0; i--) {
            strides[i] = prod;
            prod *= shape[i];
        }
        return strides;
    }

    const std::array<int, D> strides;
    std::vector<T> data;
};

} // namespace data_structures
