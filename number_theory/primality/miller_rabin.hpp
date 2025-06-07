#pragma once

#include "misc/type_promotion.hpp"
#include "number_theory/mod_exp.hpp"

#include <cassert>
#include <chrono>
#include <concepts>
#include <random>

namespace number_theory {

template <std::integral T>
constexpr bool witness(T a, T n, T u, T t) {
    T x = mod_exp(a, u, n);
    for (T i = 0; i < t; i++) {
        T now_x = (T)((promote_t<T>)x * x % n);
        if (now_x == 1 && x != n - 1 && x != 1) {
            return true;
        }
        x = now_x;
    }
    return x != 1;
}

template <std::integral T>
bool is_prime(T n, int iters, std::mt19937_64 rng) {
    assert(0 < n);
    if (n == 1) {
        return false;
    }
    T u = n - 1, t = 0;
    while (u % 2 == 0) {
        u /= 2, t += 1;
    }
    for (int iter = 0; iter < iters; iter++) {
        T a = 1 + rng() % (n - 1);
        if (witness(a, n, u, t)) {
            return false;
        }
    }
    return true;
}

template <std::integral T>
bool is_prime(T n, int iters) {
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    return is_prime(n, iters, rng);
}

template <std::integral T>
bool is_prime(T n) {
    assert(0 < n);
    static constexpr std::initializer_list<T> bases = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n == 1) {
        return false;
    }
    T u = n - 1, t = 0;
    while (u % 2 == 0) {
        u /= 2, t += 1;
    }
    for (auto a : bases) {
        if (n == a) {
            return true;
        } else if (witness(a, n, u, t)) {
            return false;
        }
    }
    return true;
};

} // namespace number_theory
