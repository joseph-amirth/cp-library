#pragma once

#include <vector>

template<typename mint>
struct binomial {
    std::vector<mint> fact, inv_fact;

    binomial(int n) : fact(n + 1), inv_fact(n + 1) {
        const int M = mint::get_m();
        fact[0] = inv_fact[0] = 1;
        for (int i = 1; i <= n; i++) {
            fact[i] = i * fact[i - 1];
            inv_fact[i] = -(M / i) * inv_fact[M % i];
        }
        for (int i = 1; i <= n; i++) {
            inv_fact[i] *= inv_fact[i - 1];
        }
    }

    mint operator()(int n, int k) const {
        if (0 <= n && n < int(fact.size()) && 0 <= k && k <= n) {
            return fact[n] * inv_fact[k] * inv_fact[n - k];
        }
        return mint(0);
    }
};