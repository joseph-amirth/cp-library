#pragma once

#include <random>
#include <chrono>
#include <cassert>

#include "../miscellaneous/type-promotion.hpp"
#include "modular-arithmetic.hpp"

namespace math {
    template<typename T>
    bool naive_primality_test(T x) {
        for (int i = 2; (T) i * i <= x; i++) {
            if (x % i == 0) {
                return false;
            }
        }
        return x != 1;
    }

    template<typename T>
    bool witness(T a, T n, T u, T t) {
        T x = modexp(a, u, n);
        for (int i = 0; i < t; i++) {
            T now_x = (promote_t<T>) x * x % n;
            if (now_x == 1 && x != n - 1 && x != 1) {
                return true;
            }
            x = now_x;
        }
        return x != 1;
    }

    template<int no_of_iter, typename T>
    bool randomized_miller_rabin(T n) {
        static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
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

    template<typename T>
    bool deterministic_miller_rabin(T n) {
        static const auto bases = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
        T u = n - 1, t = 0;
        while (u % 2 == 0) {
            u /= 2, t += 1;
        }
        for (auto a: bases) {
            if (n == a) {
                return true;
            } else if (witness((T) a, n, u, t)) {
                return false;
            }
        }
        return true;
    };

    template<int no_of_iter, typename T>
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
}