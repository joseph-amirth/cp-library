#pragma once

#include <vector>

namespace polynomials {

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

} // namespace polynomials
