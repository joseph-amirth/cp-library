#pragma once

#include <cassert>
#include <chrono>
#include <concepts>
#include <iostream>
#include <random>

#include "../miscellaneous/type_promotion.hpp"
#include "modular_arithmetic.hpp"

namespace number_theory {

template <std::integral T>
constexpr bool naive_primality_test(T x) {
    for (T i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            return false;
        }
    }
    return x != 1;
}

template <std::integral T>
constexpr bool witness(T a, T n, T u, T t) {
    T x = mod_exp(a, u, n);
    for (T i = 0; i < t; i++) {
        T now_x = (promote_t<T>)x * x % n;
        if (now_x == 1 && x != n - 1 && x != 1) {
            return true;
        }
        x = now_x;
    }
    return x != 1;
}

template <int no_of_iter, std::integral T>
bool randomized_miller_rabin(T n) {
    assert(0 < n);
    static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    if (n == 1) {
        return false;
    }
    T u = n - 1, t = 0;
    while (u % 2 == 0) {
        u /= 2, t += 1;
    }
    for (int iter = 0; iter < no_of_iter; iter++) {
        T a = 1 + rng() % (n - 1);
        if (witness(a, n, u, t)) {
            return false;
        }
    }
    return true;
}

template <std::integral T>
bool deterministic_miller_rabin(T n) {
    assert(0 < n);
    static constexpr auto bases = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
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
        } else if (witness((T)a, n, u, t)) {
            return false;
        }
    }
    return true;
};

template <int no_of_iter, std::integral T>
T random_prime(T l, T r) {
    assert(0 < l <= r);
    static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    for (int iter = 0; iter < no_of_iter; iter++) {
        T n = l + rng() % (r - l + 1);
        if (randomized_miller_rabin<30>(n)) {
            return n;
        }
    }
    return T(0);
}

} // namespace number_theory
