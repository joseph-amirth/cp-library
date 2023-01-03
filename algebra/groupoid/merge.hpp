#pragma once

#include <algorithm>

namespace algebra {

template <typename Container, typename Compare>
struct merge_monoid {
    using value_type = Container;
    using compare_type = Compare;
    using is_commutative = std::true_type;

    compare_type compare;

    merge_monoid(compare_type compare = compare_type()) : compare(compare) {}

    value_type e() {
        return value_type();
    }

    value_type op(const value_type &x, const value_type &y) {
        value_type result;
        std::merge(std::begin(x), std::end(x), std::begin(y), std::end(y), std::inserter(result, std::end(result)), compare);
        return result;
    }
};

template <typename T, typename Compare>
struct merge_monoid<std::vector<T>, Compare> {
    using value_type = std::vector<T>;
    using compare_type = Compare;
    using is_commutative = std::true_type;

    compare_type compare;

    merge_monoid(compare_type compare = compare_type()) : compare(compare) {}

    value_type e() {
        return value_type();
    }

    value_type op(const value_type &x, const value_type &y) {
        value_type result;
        result.reserve(x.size() + y.size());
        std::merge(std::begin(x), std::end(x), std::begin(y), std::end(y), std::back_inserter(result), compare);
        return result;
    }
};

}
