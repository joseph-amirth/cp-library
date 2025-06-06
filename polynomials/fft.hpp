#pragma once

#include <cmath>
#include <vector>

namespace polynomials {

namespace fft {

struct my_complex {
    double x, y;

    my_complex() : x(0), y(0) {}

    my_complex(double x) : x(x), y(0) {}

    my_complex(double x, double y) : x(x), y(y) {}

    friend my_complex operator+(const my_complex &a, const my_complex &b) {
        return my_complex(a.x + b.x, a.y + b.y);
    }

    friend my_complex operator-(const my_complex &a, const my_complex &b) {
        return my_complex(a.x - b.x, a.y - b.y);
    }

    friend my_complex operator*(const my_complex &a, const my_complex &b) {
        return my_complex(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
    }

    my_complex conj() const {
        return my_complex(x, -y);
    }
};

std::vector<my_complex> root = {{0, 0},
                                {1, 0}};

std::vector<int> rev = {0, 1 << 30};

void compute(int lg) {
    static int computed = 1;
    if (lg <= computed)
        return;
    rev.resize(1 << lg);
    for (int i = (1 << computed); i < (1 << lg); i++) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << 30);
    }
    root.resize(1 << lg);
    for (int k = computed; k < lg; k++) {
        double angle = std::numbers::pi / (1 << k);
        my_complex z(cos(angle), sin(angle));
        for (int i = (1 << (k - 1)); i < (1 << k); i++) {
            root[i << 1] = root[i];
            root[i << 1 | 1] = root[i] * z;
        }
    }
    computed = lg;
}

void fft(std::vector<my_complex> &a) {
    int n = int(a.size()), lg = 32 - __builtin_clz(n) - 1;
    compute(lg);
    int shift = 31 - lg;
    for (int i = 0; i < n; i++) {
        if (i < (rev[i] >> shift)) {
            std::swap(a[i], a[rev[i] >> shift]);
        }
    }
    for (int k = 1; k < n; k <<= 1) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                auto z = a[i + j + k] * root[j + k];
                a[i + j + k] = a[i + j] - z;
                a[i + j] = a[i + j] + z;
            }
        }
    }
}

template <typename T_in, typename T_out = T_in>
std::vector<T_out> convolution(const std::vector<T_in> &a, const std::vector<T_in> &b) {
    int n = 1, szs = (int)(a.size() + b.size());
    while (n < szs) {
        n <<= 1;
    }
    std::vector<my_complex> f(n), g(n);
    for (int i = 0; i < n; i++) {
        double real = (double)(i < (int)a.size() ? a[i] : 0);
        double imag = (double)(i < (int)b.size() ? b[i] : 0);
        f[i] = my_complex(real, imag);
    }
    fft(f);
    for (int i = 0; i < n; i++) {
        int j = (n - i) & (n - 1);
        g[i] = (f[j] * f[j] - (f[i] * f[i]).conj()) * my_complex(0, -0.25 / n);
    }
    fft(g);
    std::vector<T_out> res(n);
    for (int i = 0; i < n; i++) {
        if (std::is_integral<T_out>::value) {
            res[i] = llround(g[i].x);
        } else {
            res[i] = g[i].x;
        }
    }
    return res;
}

template <typename T_in, typename T_out = T_in>
std::vector<T_out> convolution_mod(const std::vector<T_in> &a, const std::vector<T_in> &b, int M) {
    int S = int(sqrt(M)), n = 1;
    while (n < a.size() + b.size()) {
        n <<= 1;
    }
    std::vector<my_complex> f(n), g(n);
    for (int i = 0; i < n; i++) {
        if (i < a.size())
            f[i] = my_complex(a[i] % S, a[i] / S);
        if (i < b.size())
            g[i] = my_complex(b[i] % S, b[i] / S);
    }
    fft(f), fft(g);
    std::vector<my_complex> h0(n), h1(n);
    for (int i = 0; i < n; i++) {
        int j = (n - i) & (n - 1);
        auto f_small = (f[j] + f[i].conj()) * my_complex(0.5 / n, 0);
        auto f_large = (f[j] - f[i].conj()) * my_complex(0, -0.5 / n);
        auto g_small = (g[j] + g[i].conj()) * my_complex(0.5, 0);
        auto g_large = (g[j] - g[i].conj()) * my_complex(0, -0.5);
        h0[i] = f_small * g_small + my_complex(0, 1) * (f_small * g_large + f_large * g_small);
        h1[i] = f_large * g_large;
    }
    fft(h0), fft(h1);
    std::vector<T_out> res(n);
    for (int i = 0; i < n; i++) {
        T_out t1 = llround(h0[i].x) % M;
        T_out t2 = llround(h0[i].y) % M * S % M;
        T_out t3 = llround(h1[i].x) % M * S * S % M;
        res[i] = t1;
        if ((res[i] += t2) >= M)
            res[i] -= M;
        if ((res[i] += t3) >= M)
            res[i] -= M;
    }
    return res;
}

template <typename T>
void normalize(std::vector<T> &a) {
    for (int i = int(a.size()) - 1; i >= 0; i--) {
        if (a[i]) {
            a.resize(i + 1);
            return;
        }
    }
    a.clear();
}

} // namespace fft

} // namespace polynomials
