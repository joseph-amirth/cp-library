#pragma once

#include "../concepts.hpp"
#include <concepts>
#include <functional>
#include <vector>

namespace algebra {

namespace semigroups {

template <typename T, std::strict_weak_order<T, T> C = std::less<>>
struct extremum_index {
    using value_type = int;

    C compare;
    const std::vector<T> &v;

    extremum_index(const std::vector<T> &v) : v(v) {}

    extremum_index(C compare, const std::vector<T> &v) : compare(compare), v(v) {}

    int op(int i, int j) {
        return compare(v[i], v[j]) ? i : j;
    }
};

} // namespace semigroups

template <typename T, typename C>
struct is_commutative<semigroups::extremum_index<T, C>> : std::true_type {};

template <typename T, typename C>
struct is_idempotent<semigroups::extremum_index<T, C>> : std::true_type {};

} // namespace algebra
