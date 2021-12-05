#pragma once

#include <vector>

template<typename T>
std::vector<T> subset_convolution(int n, const std::vector<T> &f, const std::vector<T> &g) {
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