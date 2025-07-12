#pragma once

#include "algebra/concepts.hpp"
#include "algebra/monoids/product.hpp"

#include <type_traits>

namespace algebra {

namespace groups {

template <typename T>
struct product : public monoids::product<T> {
    using typename monoids::product<T>::value_type;
    using monoids::product<T>::id;

    value_type inv(value_type a) {
        return id() / a;
    }
};

} // namespace groups

template <typename T>
struct is_commutative<groups::product<T>> : std::true_type {};

} // namespace algebra
