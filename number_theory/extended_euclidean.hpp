#pragma once

#include <cassert>
#include <concepts>
#include <optional>
#include <tuple>
#include <vector>

#include "misc/type_promotion.hpp"

namespace number_theory {

template <std::integral T>
std::tuple<T, T, T> ext_euclid(T a, T b) {
    T x = 1, y = 0, x1 = 0, y1 = 1;
    while (a != 0) {
        T q = b / a;
        std::tie(x, x1) = std::make_pair(-q * x + x1, x);
        std::tie(y, y1) = std::make_pair(-q * y + y1, y);
        std::tie(a, b) = std::make_pair(-q * a + b, a);
    }
    if (b < 0) {
        b = -b, x1 = -x1, y1 = -y1;
    }
    return std::make_tuple(b, x1, y1);
}

template <typename T>
T gcd(T a, T b) {
    return std::get<0>(ext_euclid(a, b));
}

template <typename T>
T mod_inverse(T a, T m) {
    assert(m > 0);
    auto [g, x, y] = ext_euclid(a, m);
    if (g != 1) {
        return -1;
    } else {
        return ((x %= m) < 0 ? x + m : x);
    }
}

template <typename T>
std::optional<std::pair<T, T>> solve_linear_congruence(T a, T b, T n) {
    assert(n > 0);
    T d = gcd(a, n);
    if (b % d != 0) {
        return std::nullopt;
    } else {
        a /= d, b /= d, n /= d;
        T sol = (T)((promote_t<T>)mod_inverse(a, n) * b % n);
        if (sol < 0) {
            sol += n;
        }
        return std::make_pair(sol, d);
    }
}

std::pair<long long, long long> crt(long long r0, long long m0, long long r1, long long m1) {
    assert(m0 > 0 && m1 > 0);
    auto [g, a0, a1] = ext_euclid(m0, m1);
    if ((r0 - r1) % g != 0) {
        return std::make_pair(-1, -1);
    }
    long long m = m0 * m1 / g;
    long long r = r1 * a0 % m * (m0 / g) % m + r0 * a1 % m + (m1 / g) % m;
    if ((r %= m) < 0) {
        r += m;
    }
    return std::make_pair(r, m);
}

template <typename T>
std::pair<long long, long long> crt(const std::vector<T> &r, const std::vector<T> &m) {
    assert(r.size() == m.size());
    int n = (int)r.size();
    long long x = 0, N = 1;
    for (int i = 0; i < n && N > 0; i++) {
        std::tie(x, N) = crt(x, N, r[i], m[i]);
    }
    return std::make_pair(x, N);
}

} // namespace number_theory
