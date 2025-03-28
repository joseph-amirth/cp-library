#pragma once

#include <vector>

namespace polynomials {

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

} // namespace polynomials
