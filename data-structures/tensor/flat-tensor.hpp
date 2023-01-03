#pragma once

#include <vector>
#include <array>
#include <algorithm>
#include <cassert>
#include <memory>

namespace data_structures {

template <typename T>
struct flat_tensor {
    using value_type = T;

    int origin_dims, n_dims;
    int offset;
    std::shared_ptr<int> strides, shape;
    std::shared_ptr<value_type> data;

    flat_tensor() : origin_dims(), n_dims(), offset(), strides(), shape(), data() {}

    template <typename Range>
    flat_tensor(Range &&shape, value_type init = value_type()) : origin_dims(std::size(shape)),
                                                                 n_dims(std::size(shape)),
                                                                 offset() {
        this->strides = std::shared_ptr<int>(new int[n_dims]);
        this->shape = std::shared_ptr<int>(new int[n_dims]);

        int *shape_ptr = this->shape.get();

        int sz = 1;
        for (auto dim : shape) {
            sz *= dim;
            *shape_ptr++ = dim;
        }

        this->data = std::shared_ptr<value_type>(new value_type[sz]);
        std::fill(this->data.get(), this->data.get() + sz, init);

        int *strides_ptr = this->strides.get();
        for (auto dim : shape) {
            sz /= dim;
            *strides_ptr++ = sz;
        }
    }

    flat_tensor(int origin_dims, int n_dims, int offset, std::shared_ptr<int> strides, std::shared_ptr<int> shape,
                std::shared_ptr<value_type> data) {
        this->origin_dims = origin_dims;
        this->n_dims = n_dims;
        this->offset = offset;
        this->strides = std::move(strides);
        this->shape = std::move(shape);
        this->data = std::move(data);
    }

    template <typename Container = std::vector<int>>
    Container get_shape() {
        return Container(this->shape.get() + origin_dims - n_dims, this->shape.get() + origin_dims);
    }

    template <typename Range>
    value_type &operator[](Range &&index) {
        assert(std::size(index) == n_dims);

        int *strides_ptr = strides.get() + origin_dims - n_dims;

        int req_offset = offset;
        for (int i : index) {
            req_offset += i * (*strides_ptr++);
        }

        return data.get()[req_offset];
    }

    template <typename Range>
    const value_type &operator[](Range &&index) const {
        assert(std::size(index) == n_dims);

        int *strides_ptr = strides.get() + origin_dims - n_dims;

        int req_offset = offset;
        for (int i : index) {
            req_offset += i * (*strides_ptr++);
        }

        return data.get()[req_offset];
    }

    template <typename Range>
    flat_tensor get_tensor_view(Range &&index) {
        assert(std::size(index) <= n_dims);

        int *strides_ptr = strides.get() + origin_dims - n_dims;

        int req_offset = offset;
        for (int i : index) {
            req_offset += i * (*strides_ptr++);
        }

        return flat_tensor(origin_dims, n_dims - std::size(index), req_offset, strides, shape, data);
    }

    flat_tensor get_original_tensor() const {
        return flat_tensor(origin_dims, origin_dims, 0, strides, shape, data);
    }
};

}
