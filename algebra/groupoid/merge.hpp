#pragma once

#include <algorithm>

namespace algebra {

template <typename Container>
struct merge_monoid {
    using value_type = Container;
    using is_commutative = std::true_type;

    static value_type e() {
        return value_type();
    }

    static value_type op(const value_type &x, const value_type &y) {
        value_type result;
        std::merge(std::begin(x), std::end(x), std::begin(y), std::end(y), std::inserter(result, std::end(result)));
        return result;
    }
};

template <typename T>
struct merge_monoid<std::vector<T>> {
    using value_type = std::vector<T>;
    using is_commutative = std::true_type;

    static value_type e() {
        return value_type();
    }

    static value_type op(const value_type &x, const value_type &y) {
        value_type result;
        result.reserve(x.size() + y.size());
        std::merge(std::begin(x), std::end(x), std::begin(y), std::end(y), std::back_inserter(result));
        return result;
    }
};

}
