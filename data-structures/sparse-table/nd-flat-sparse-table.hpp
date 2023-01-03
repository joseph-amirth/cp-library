#pragma once

#include "../tensor/flat-tensor.hpp"

namespace data_structures {

template <typename Semigroup>
struct nd_flat_sparse_table {
    using groupoid = Semigroup;
    using value_type = typename groupoid::value_type;

    int n_dims;
    flat_tensor<value_type> table;
    groupoid semigroup;

    nd_flat_sparse_table() {}

    template <typename T>
    nd_flat_sparse_table(const flat_tensor<T> &t, groupoid semigroup = groupoid()) : n_dims(t.n_dims),
                                                                                     semigroup(semigroup) {
        std::vector<int> mat_shape(2 * n_dims);
        for (int i = 0; i < n_dims; i++) {
            mat_shape[2 * i] = 32 - __builtin_clz(t.shape.get()[i]);
            mat_shape[2 * i + 1] = t.shape.get()[i];
        }
        table = flat_tensor<T>(std::move(mat_shape));
        auto build = [&](auto &&self, int t_offset, int mat_offset, int d) -> void {
            if (d == n_dims) {
                table.data.get()[mat_offset] = T(t.data.get()[t_offset]);
            } else {
                int lg = table.shape.get()[2 * d], n = table.shape.get()[2 * d + 1];
                for (int i = 0; i < n; i++) {
                    self(self, t_offset + i * t.strides.get()[d], mat_offset + i * table.strides.get()[2 * d + 1],
                         d + 1);
                }
                for (int j = 1; j < lg; j++) {
                    for (int i = 0; i + (1 << j) <= n; i++) {
                        auto result_offset =
                                mat_offset + j * table.strides.get()[2 * d] + i * table.strides.get()[2 * d + 1];
                        auto a_offset =
                                mat_offset + (j - 1) * table.strides.get()[2 * d] + i * table.strides.get()[2 * d + 1];
                        auto b_offset = mat_offset + (j - 1) * table.strides.get()[2 * d] +
                                        (i + (1 << (j - 1))) * table.strides.get()[2 * d + 1];
                        for (int k = 0; k < table.strides.get()[2 * d + 1]; k++) {
                            table.data.get()[result_offset + k] = semigroup.op(table.data.get()[a_offset + k],
                                                                               table.data.get()[b_offset + k]);
                        }
                    }
                }
            }
        };
        build(build, 0, 0, 0);
    }

    value_type query(std::initializer_list<std::pair<int, int>> &&range) {
        int i = 0;
        for (auto [l, r] : range) {
            assert(0 <= l && l <= r && r < table.shape.get()[2 * i + 1]);
            i += 1;
        }
        auto recurse = [&](auto &&self, auto it, int d, int mat_offset) -> value_type {
            if (it != range.end()) {
                auto [l, r] = *it;
                int j = 32 - __builtin_clz(r - l + 1) - 1;
                return semigroup.op(
                        self(self, std::next(it), d + 1,
                             mat_offset + j * table.strides.get()[2 * d] + l * table.strides.get()[2 * d + 1]),
                        self(self, std::next(it), d + 1,
                             mat_offset + j * table.strides.get()[2 * d] +
                             (r - (1 << j) + 1) * table.strides.get()[2 * d + 1]));
            } else {
                return table.data.get()[mat_offset];
            }
        };
        return recurse(recurse, std::begin(range), 0, 0);
    }
};

}
