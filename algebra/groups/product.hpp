#pragma once

#include "../concepts.hpp"
#include <type_traits>

namespace algebra {

namespace groups {

template <typename T>
struct product {
    using value_type = T;

    product() {}

    value_type id() { return value_type(1); }

    value_type op(value_type a, value_type b) {
        return a * b;
    }

    value_type inv(value_type a) {
        return id() / a;
    }
};

} // namespace groups
  //
template <typename T>
struct is_commutative<groups::product<T>> : std::true_type {};

} // namespace algebra
