#pragma once

#include <functional>

namespace algebra {

template <typename T, typename Compare = std::less<>>
struct extremum_semigroup {
    using value_type = T;
    using compare_type = Compare;
    using is_commutative = std::true_type;
    using is_idempotent = std::true_type;

    static compare_type compare;

    static value_type op(const value_type &x, const value_type &y) {
        return compare(x, y) ? x : y;
    }
};

template <typename T, typename Compare>
Compare extremum_semigroup<T, Compare>::compare;

}
