#pragma once

#include <vector>
#include <cassert>

namespace math {
    constexpr int mod_exp(long long x, long long n, int m) {
        assert(0 <= n && 0 < m);
        if ((x %= m) < 0) {
            x += m;
        }
        long long ans = 1;
        for (; n; n /= 2) {
            if (n & 1) {
                ans = ans * x % m;
            }
            x = x * x % m;
        }
        return ans;
    }

    template<typename T>
    constexpr T primitive_root(T n) {
        assert(1 < n);

        T factors[20] = {};
        int no_factors = 0;

        T x = n - 1;
        for (int i = 2; (T) i * i <= x; i++) {
            if (x % i == 0) {
                factors[no_factors++] = i;
                while (x % i == 0) {
                    x /= i;
                }
            }
        }
        if (x > 1) {
            factors[no_factors++] = x;
        }

        for (int i = 1; i < n; i++) {
            bool is_primitive_root = true;
            for (int j = 0; j < no_factors; j++) {
                is_primitive_root &= mod_exp(i, (n - 1) / factors[j], n) != 1;
            }
            if (is_primitive_root) {
                return i;
            }
        }
        return -1;
    }
}