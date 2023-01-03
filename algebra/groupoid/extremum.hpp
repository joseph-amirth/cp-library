#pragma once

#include <functional>

namespace algebra {

template <typename T, typename Compare = std::less<>>
struct extremum_semigroup {
    using value_type = T;
    using compare_type = Compare;
    using is_commutative = std::true_type;
    using is_idempotent = std::true_type;

    compare_type compare;

    extremum_semigroup(compare_type compare = compare_type()) : compare(compare) {}

    value_type op(const value_type &x, const value_type &y) {
        return compare(x, y) ? x : y;
    }
};

template <typename T, typename Compare = std::less<>>
struct extremum_monoid : extremum_semigroup<T, Compare> {
    using value_type = T;
    using compare_type = Compare;

    value_type id;

    extremum_monoid(value_type id, compare_type compare = compare_type()) : id(id), extremum_semigroup<T, Compare>(compare) {}

    value_type e() {
        return id;
    }
};

}
