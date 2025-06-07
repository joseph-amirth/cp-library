#pragma once

#include "number_theory/factorize/pollard_rho.hpp"
#include "number_theory/mod_exp.hpp"

#include <cassert>
#include <concepts>

namespace number_theory {

template <std::integral T>
constexpr T primitive_root(T n) {
    assert(1 < n);

    auto factors = factorize(n - 1);
    factors.erase(unique(factors.begin(), factors.end()), factors.end());

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
