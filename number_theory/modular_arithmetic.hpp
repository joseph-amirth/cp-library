#pragma once

#include "misc/type_promotion.hpp"
#include <cassert>
#include <concepts>

namespace number_theory {

template <std::integral T>
constexpr T mod_exp(T x, long long n, T m) {
    assert(0 <= n && 0 < m);
    if ((x %= m) < 0) {
        x += m;
    }
    T ans = 1;
    for (; n > 0; n /= 2) {
        if (n & 1) {
            ans = (promote_t<T>)ans * x % m;
        }
        x = (promote_t<T>)x * x % m;
    }
    return ans;
}

template <std::integral T>
constexpr T primitive_root(T n) {
    assert(1 < n);

    std::size_t num_factors = 0;
    T x = n - 1, factors[20] = {};
    for (T i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            factors[num_factors++] = i;
            while (x % i == 0) {
                x /= i;
            }
        }
    }
    if (x > 1) {
        factors[num_factors++] = x;
    }

    for (T i = 1; i < n; i++) {
        bool is_primitive_root = true;
        for (std::size_t j = 0; j < num_factors; j++) {
            is_primitive_root &= mod_exp(i, (n - 1) / factors[j], n) != 1;
        }
        if (is_primitive_root) {
            return i;
        }
    }
    return T(0);
}

} // namespace number_theory
