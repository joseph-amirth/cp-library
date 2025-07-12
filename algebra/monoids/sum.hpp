#pragma once

#include "algebra/concepts.hpp"

#include <type_traits>

namespace algebra {

namespace monoids {

template <typename T>
struct sum {
    using value_type = T;

    sum() {}

    value_type id() { return value_type(0); }

    value_type op(value_type a, value_type b) {
        return a + b;
    }
};

} // namespace monoids

template <typename T>
struct is_commutative<monoids::sum<T>> : std::true_type {};

} // namespace algebra
