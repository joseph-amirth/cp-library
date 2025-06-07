#pragma once

#include "misc/type_promotion.hpp"
#include "number_theory/primality/miller_rabin.hpp"

#include <chrono>
#include <concepts>
#include <cstdlib>
#include <numeric>
#include <random>
#include <vector>

namespace number_theory {

template <std::integral T>
T rho(T n, T x0, T c) {
    T x = x0, y = x0;
    while (true) {
        x = (T)(((promote_t<T>)x * x + c) % n);
        y = (T)(((promote_t<T>)y * y + c) % n);
        y = (T)(((promote_t<T>)y * y + c) % n);
        T g = std::gcd(std::abs(x - y), n);
        if (g != 1) {
            return g;
        }
    }
}

template <std::integral T>
void factorize_impl(T n, std::vector<T> &result, std::mt19937_64 &rng) {
    if (is_prime(n)) {
        result.push_back(n);
        return;
    }
    while (true) {
        T x0 = 2 + rng() % (n - 2);
        T c = 1 + rng() % (n - 1);
        T g = rho(n, x0, c);
        if (g != n) {
            factorize_impl(g, result, rng);
            factorize_impl(n / g, result, rng);
            break;
        }
    }
}

template <std::integral T>
std::vector<T> factorize(T n, std::mt19937_64 &rng) {
    std::vector<T> result;
    while (n % 2 == 0) {
        result.push_back(2);
        n /= 2;
    }
    if (n == 1) {
        return result;
    }
    factorize_impl(n, result, rng);
    return result;
}

template <std::integral T>
std::vector<T> factorize(T n) {
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    return factorize(n, rng);
}

} // namespace number_theory
