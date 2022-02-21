#pragma

#include "tensor/flat-tensor.hpp"

template <typename T, int D = 1, typename F = std::function<T(const T&, const T&)>>
struct nd_flat_sparse_table {
    static_assert(D > 0, "Dimension must be positive");

    flat_tensor<T, 2 * D> mat;
    F f;

    nd_flat_sparse_table() : mat(), f() {}

    template <typename U>
    nd_flat_sparse_table(const flat_tensor<U, D> &t, F &&f) : f(f) {
        std::array<int, 2 * D> mat_shape;
        for (int i = 0; i < D; i++) {
            mat_shape[2 * i] = 32 - __builtin_clz(t.shape[i]);
            mat_shape[2 * i + 1] = t.shape[i];
        }
        mat = flat_tensor<T, 2 * D>(mat_shape);
        auto build = [&](int t_offset, int mat_offset, int d, auto &&self) -> void {
            if (d == D) {
                mat[mat_offset] = T(t[t_offset]);
            } else {
                int lg = mat.shape[2 * d], n = mat.shape[2 * d + 1];
                for (int i = 0; i < n; i++) {
                    self(t_offset + i * t.strides[d], mat_offset + i * mat.strides[2 * d + 1], d + 1, self);
                }
                for (int j = 1; j < lg; j++) {
                    for (int i = 0; i + (1 << j) <= n; i++) {
                        auto result_offset = mat_offset + j * mat.strides[2 * d] + i * mat.strides[2 * d + 1];
                        auto a_offset = mat_offset + (j - 1) * mat.strides[2 * d] + i * mat.strides[2 * d + 1];
                        auto b_offset = mat_offset + (j - 1) * mat.strides[2 * d] + (i + (1 << (j - 1))) * mat.strides[2 * d + 1];
                        for (int k = 0; k < mat.strides[2 * d + 1]; k++) {
                            mat[result_offset + k] = f(mat[a_offset + k], mat[b_offset + k]);
                        }
                    }
                }
            }
        };
        build(0, 0, 0, build);
    }

    T query(const std::initializer_list<std::pair<int, int>> &range) {
        auto recurse = [&](auto it, int d, int mat_offset, auto &&self) -> T {
            if (it != range.end()) {
                auto [l, r] = *it;
                assert(0 <= l && l <= r && r < mat.shape[2 * d + 1]);
                int j = 32 - __builtin_clz(r - l + 1) - 1;
                return f(self(std::next(it), d + 1, mat_offset + j * mat.strides[2 * d] + l * mat.strides[2 * d + 1], self),
                         self(std::next(it), d + 1, mat_offset + j * mat.strides[2 * d] + (r - (1 << j) + 1) * mat.strides[2 * d + 1], self));
            } else {
                return mat[mat_offset];
            }
        };
        return recurse(range.begin(), 0, 0, recurse);
    }
};