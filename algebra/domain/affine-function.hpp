#pragma once

namespace algebra {

template <typename T>
struct affine_function {
    T a, b;

    explicit affine_function(T a = T(1), T b = T(0)) : a(a), b(b) {}

    affine_function inv() const {
        T inv_a = T(1) / a;
        return affine_function(inv_a, -b * inv_a);
    }

    affine_function operator()(const affine_function &other) const {
        return affine_function(a * other.a, a * other.b + b);
    }

    T operator()(const T &x) const {
        return a * x + b;
    }
};

}
