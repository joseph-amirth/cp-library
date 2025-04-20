#pragma once

#include "../concepts.hpp"
#include <concepts>
#include <functional>

namespace algebra {

namespace monoids {

template <typename T, std::strict_weak_order<T, T> C = std::less<>>
struct extremum {
    using value_type = T;

    C compare;
    value_type _id;

    extremum(C compare, value_type _id) : compare(compare), _id(_id) {}

    extremum(value_type _id) : extremum(C(), _id) {}

    value_type id() {
        return _id;
    }

    value_type op(value_type a, value_type b) {
        return compare(a, b) ? a : b;
    }
};

} // namespace monoids

template <typename T, typename C>
struct is_commutative<monoids::extremum<T, C>> : std::true_type {};

template <typename T, typename C>
struct is_idempotent<monoids::extremum<T, C>> : std::true_type {};

} // namespace algebra
