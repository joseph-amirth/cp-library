#pragma once

#include "../concepts.hpp"
#include <type_traits>

namespace algebra {

namespace groups {

template <typename T>
struct sum {
    using value_type = T;

    sum() {}

    value_type id() { return value_type(0); }

    value_type op(value_type a, value_type b) {
        return a + b;
    }

    value_type inv(value_type a) {
        return -a;
    }
};

} // namespace groups

template <typename T>
struct is_commutative<groups::sum<T>> : std::true_type {};

} // namespace algebra
