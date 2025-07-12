#pragma once

#include "algebra/concepts.hpp"

#include <type_traits>

namespace algebra {

namespace monoids {

template <typename T>
struct product {
    using value_type = T;

    product() {}

    value_type id() { return value_type(1); }

    value_type op(value_type a, value_type b) {
        return a * b;
    }
};

} // namespace monoids

template <typename T>
struct is_commutative<monoids::product<T>> : std::true_type {};

} // namespace algebra
