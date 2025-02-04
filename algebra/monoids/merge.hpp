#pragma once

#include <algorithm>
#include <concepts>
#include <ranges>
#include <vector>

#include "../concepts.hpp"

namespace algebra {

template <std::ranges::range R,
          std::strict_weak_order<std::ranges::range_value_t<R>, std::ranges::range_value_t<R>> C>
struct merge {
    using value_type = R;

    C compare;

    merge(C compare = C()) : compare(compare) {}

    value_type id() {
        return value_type();
    }

    value_type op(const value_type &x, const value_type &y) {
        value_type result;
        std::merge(std::begin(x), std::end(x), std::begin(y), std::end(y), std::inserter(result, std::end(result)), compare);
        return result;
    }
};

template <typename T, std::strict_weak_order<T, T> C>
struct merge<std::vector<T>, C> {
    using value_type = std::vector<T>;

    C compare;

    merge(C compare = C()) : compare(compare) {}

    value_type id() {
        return value_type();
    }

    value_type op(const value_type &x, const value_type &y) {
        value_type result;
        result.reserve(x.size() + y.size());
        std::merge(std::begin(x), std::end(x), std::begin(y), std::end(y), std::back_inserter(result), compare);
        return result;
    }
};

template <typename R, typename C>
struct is_commutative<merge<R, C>> : std::true_type {};

} // namespace algebra
