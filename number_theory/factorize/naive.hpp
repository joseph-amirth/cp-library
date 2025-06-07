#pragma once

#include <concepts>
#include <vector>

namespace number_theory {

template <std::integral T>
constexpr std::vector<T> factorize(T n) {
    std::vector<T> factors;
    for (T i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }
    if (n > 1) {
        factors.push_back(n);
    }
    return factors;
}

} // namespace number_theory
