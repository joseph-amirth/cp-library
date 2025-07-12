#pragma once

#include "algebra/concepts.hpp"
#include "algebra/monoids/sum.hpp"

#include <type_traits>

namespace algebra {

namespace groups {

template <typename T>
struct sum : public monoids::sum<T> {
    using typename monoids::sum<T>::value_type;

    value_type inv(value_type a) {
        return -a;
    }
};

} // namespace groups

template <typename T>
struct is_commutative<groups::sum<T>> : std::true_type {};

} // namespace algebra
