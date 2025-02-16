#pragma once

#include <vector>

namespace polynomials {

template <typename T>
std::vector<T> subset_convolution(const std::vector<T> &f, const std::vector<T> &g) {
    int n = 32 - __builtin_clz(f.size()) - 1;
    assert(f.size() == (1u << n) && g.size() == (1u << n));
    std::vector<std::vector<T>> f_hat(n + 1, std::vector<T>(1 << n));
    std::vector<std::vector<T>> g_hat(n + 1, std::vector<T>(1 << n));
    for (int mask = 0; mask < (1 << n); mask++) {
        f_hat[__builtin_popcount(mask)][mask] = f[mask];
        g_hat[__builtin_popcount(mask)][mask] = g[mask];
    }
    for (int k = 0; k <= n; k++) {
        for (int j = 0; j < n; j++) {
            for (int mask = 0; mask < (1 << n); mask++) {
                if (mask >> j & 1) {
                    f_hat[k][mask] += f_hat[k][mask ^ (1 << j)];
                    g_hat[k][mask] += g_hat[k][mask ^ (1 << j)];
                }
            }
        }
    }
    std::vector<std::vector<T>> h(n + 1, std::vector<T>(1 << n));
    for (int k = 0; k <= n; k++) {
        for (int j = 0; j <= k; j++) {
            for (int mask = 0; mask < (1 << n); mask++) {
                h[k][mask] += f_hat[j][mask] * g_hat[k - j][mask];
            }
        }
    }
    for (int k = 0; k <= n; k++) {
        for (int j = 0; j < n; j++) {
            for (int mask = 0; mask < (1 << n); mask++) {
                if (mask >> j & 1) {
                    h[k][mask] -= h[k][mask ^ (1 << j)];
                }
            }
        }
    }
    std::vector<T> result(1 << n);
    for (int mask = 0; mask < (1 << n); mask++) {
        result[mask] = h[__builtin_popcount(mask)][mask];
    }
    return result;
}

} // namespace polynomials
