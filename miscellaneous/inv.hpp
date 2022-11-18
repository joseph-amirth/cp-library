#pragma once

#include "../helper-macros/define-has-method.hpp"

DEFINE_HAS_METHOD(inv)

template <typename T>
T inv_impl(const T &x, std::true_type) {
    return x.inv();
}

template <typename T>
T inv_impl(const T &x, std::false_type) {
    return T(1) / x;
}

template <typename T>
T inv(const T &x) {
    return inv_impl(x, has_method_inv<T, T(void)>());
}
