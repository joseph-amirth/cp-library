#pragma once

#include "number_theory/concepts.hpp"
#include "number_theory/mod_exp.hpp"

#include <cassert>
#include <concepts>

namespace number_theory {

template <std::integral T>
constexpr T primitive_root(T n, Factorizer<T> auto factorize) {
    assert(1 < n);

    auto factors = factorize(n - 1);

    for (T i = 1; i < n; i++) {
        bool is_primitive_root = true;
        for (auto factor : factors) {
            is_primitive_root &= mod_exp(i, (n - 1) / factor, n) != 1;
        }
        if (is_primitive_root) {
            return i;
        }
    }
    return T(0);
}

} // namespace number_theory
