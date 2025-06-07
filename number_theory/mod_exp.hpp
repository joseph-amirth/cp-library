#pragma once

#include "misc/type_promotion.hpp"

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
            ans = (T)((promote_t<T>)ans * x % m);
        }
        x = (T)((promote_t<T>)x * x % m);
    }
    return ans;
}

} // namespace number_theory
