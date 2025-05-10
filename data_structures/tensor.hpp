#pragma once

#include <array>
#include <cassert>
#include <vector>

namespace data_structures {

template <typename T, int D>
struct tensor_view {
    static_assert(0 < D, "Dimension should be positive");

    template <typename T1, int D1>
    friend struct tensor_view;

    int size() const {
        return *dims_ptr;
    }

    T &operator[](int i)
        requires(D == 1)
    {
        return data_ptr[i];
    }

    const T &operator[](int i) const
        requires(D == 1)
    {
        return data_ptr[i];
    }

    tensor_view<T, D - 1> operator[](int i) {
        int offset = (*strides_ptr) * i;
        return tensor_view<T, D - 1>(dims_ptr + 1, strides_ptr + 1, data_ptr + offset);
    }

    const tensor_view<T, D - 1> operator[](int i) const
        requires(D > 1)
    {
        int offset = (*strides_ptr) * i;
        return tensor_view<T, D - 1>(dims_ptr + 1, strides_ptr + 1, data_ptr + offset);
    }

  protected:
    const int *dims_ptr, *strides_ptr;
    T *data_ptr;

    tensor_view() {}
    tensor_view(const int *dims_ptr, const int *strides_ptr, T *data_ptr) : dims_ptr(dims_ptr), strides_ptr(strides_ptr), data_ptr(data_ptr) {}
};

template <typename T, int D>
struct tensor : public tensor_view<T, D> {
    static_assert(0 < D, "Dimension should be positive");

    tensor(const std::array<int, D> &dims) : tensor_view<T, D>(), dims(dims), strides() {
        int prod = 1;
        for (int i = D - 1; i >= 0; i--) {
            strides[i] = prod;
            prod *= dims[i];
        }
        data.assign(prod, T());
        this->dims_ptr = this->dims.data();
        this->strides_ptr = this->strides.data();
        this->data_ptr = this->data.data();
    }

  private:
    std::array<int, D> dims, strides;
    std::vector<T> data;
};

} // namespace data_structures
