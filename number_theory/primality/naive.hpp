#pragma once

#include <concepts>

namespace number_theory {

template <std::integral T>
constexpr bool is_prime(T x) {
    for (T i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            return false;
        }
    }
    return x != 1;
}

} // namespace number_theory
