#pragma once

#include <vector>

namespace polynomial {

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

template <typename T>
void fwht_xor(std::vector<T> &a) {
    int n = (int)a.size();
    for (int l = 1; l < n; l <<= 1) {
        for (int i = 0; i < n; i += 2 * l) {
            for (int j = 0; j < l; j++) {
                T v = a[i + j + l];
                a[i + j + l] = a[i + j] - v;
                a[i + j] = a[i + j] + v;
            }
        }
    }
}

template <typename T>
std::vector<T> xor_convolution(std::vector<T> a, std::vector<T> b) {
    int n = 1;
    while (n < (int)a.size() && n < (int)b.size()) {
        n <<= 1;
    }
    a.resize(n), b.resize(n);
    fwht_xor(a), fwht_xor(b);
    T inv_n = 1 / T(n);
    for (int i = 0; i < n; i++) {
        a[i] *= b[i] * inv_n;
    }
    fwht_xor(a);
    return a;
}

template <typename T>
void fwht_and(std::vector<T> &a, bool invert = false) {
    int n = (int)a.size();
    for (int l = 1; l < n; l <<= 1) {
        for (int i = 0; i < n; i += 2 * l) {
            for (int j = 0; j < l; j++) {
                if (invert) {
                    a[i + j] -= a[i + j + l];
                } else {
                    a[i + j] += a[i + j + l];
                }
            }
        }
    }
}

template <typename T>
std::vector<T> and_convolution(std::vector<T> a, std::vector<T> b) {
    int n = 1;
    while (n < (int)a.size() && n < (int)b.size()) {
        n <<= 1;
    }
    a.resize(n), b.resize(n);
    fwht_and(a), fwht_and(b);
    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }
    fwht_and(a, true);
    return a;
}

template <typename T>
std::vector<T> or_convolution(std::vector<T> a, std::vector<T> b) {
    int n = 1;
    while (n < (int)a.size() && n < (int)b.size()) {
        n <<= 1;
    }
    a.resize(n), b.resize(n);
    for (int i = 0; i < n; i++) {
        int j = ~i & (n - 1);
        if (i < j) {
            swap(a[i], a[j]);
            swap(b[i], b[j]);
        }
    }
    fwht_and(a), fwht_and(b);
    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }
    fwht_and(a, true);
    for (int i = 0; i < n; i++) {
        int j = ~i & (n - 1);
        if (i < j) {
            swap(a[i], a[j]);
        }
    }
    return a;
}

} // namespace polynomial
