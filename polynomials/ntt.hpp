#pragma once

#include "algebra/mint/concepts.hpp"
#include "number_theory/factorize/naive.hpp"
#include "number_theory/mod_exp.hpp"
#include "number_theory/primitive_root.hpp"

#include <algorithm>
#include <vector>

namespace polynomials {

namespace ntt {

template <typename T>
constexpr T ntt_root(T n) {
    auto root = number_theory::primitive_root(n, [](T x) {
        return number_theory::factorize(x);
    });
    return number_theory::mod_exp(root, (n - 1) >> __builtin_ctz(n - 1), n);
}

template <typename T>
constexpr std::size_t order_of_ntt_root(T n) {
    return 1 << __builtin_ctz(n - 1);
}

template <algebra::StaticMint Mint>
struct ntt_info {
    static constexpr auto m = Mint::get_m();
    static constexpr auto root = ntt_root(m);
    static constexpr auto root_order = order_of_ntt_root(m);

    static std::vector<Mint> roots;

    static void compute_roots(std::size_t lg) {
        static std::size_t computed = 1;
        if (lg <= computed)
            return;
        roots.resize(1 << lg);
        for (std::size_t k = computed; k < lg; k++) {
            Mint z(ntt_info<Mint>::root);
            for (std::size_t i = (1 << (k + 1)); i < root_order; i <<= 1) {
                z *= z;
            }
            for (std::size_t i = (1 << (k - 1)); i < (1u << k); i++) {
                roots[i << 1] = roots[i];
                roots[i << 1 | 1] = roots[i] * z;
            }
        }
        computed = lg;
    };
};

template <algebra::StaticMint Mint>
std::vector<Mint> ntt_info<Mint>::roots = {0, 1};

namespace {

std::vector<int> rev = {0};

void compute_bit_reverse(int lg) {
    static int computed = 0;
    if (lg <= computed)
        return;
    rev.resize(1 << lg);
    for (int i = (1 << computed); i < (1 << lg); i++) {
        rev[i] = (rev[i >> 1] >> 1) ^ ((i & 1) << 30);
    }
    computed = lg;
}

} // namespace

template <typename Mint>
void ntt(std::vector<Mint> &a) {
    int n = int(a.size()), lg = 32 - __builtin_clz(n) - 1;
    compute_bit_reverse(lg);
    ntt_info<Mint>::compute_roots(lg);
    int shift = 31 - lg;
    for (int i = 0; i < n; i++) {
        if (i < (rev[i] >> shift)) {
            std::swap(a[i], a[rev[i] >> shift]);
        }
    }
    for (int k = 1; k < n; k <<= 1) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                auto z = ntt_info<Mint>::roots[j + k] * a[i + j + k];
                a[i + j + k] = a[i + j] - z;
                a[i + j] = a[i + j] + z;
            }
        }
    }
}

template <typename Mint>
std::vector<Mint> convolution(std::vector<Mint> a, std::vector<Mint> b) {
    std::size_t n = 1;
    while (n < a.size() + b.size()) {
        n <<= 1;
    }
    a.resize(n), b.resize(n);
    ntt(a), ntt(b);
    Mint n_inv = Mint(n).inv();
    for (std::size_t i = 0; i < n; i++) {
        a[i] *= b[i] * n_inv;
    }
    std::reverse(a.begin() + 1, a.end());
    return ntt(a), a;
}

template <typename Mint>
std::vector<Mint> &inplace_convolution(std::vector<Mint> &a, std::vector<Mint> b) {
    std::size_t n = 1;
    while (n < a.size() + b.size()) {
        n <<= 1;
    }
    a.resize(n), b.resize(n);
    ntt(a), ntt(b);
    Mint n_inv = Mint(n).inv();
    for (std::size_t i = 0; i < n; i++) {
        a[i] *= b[i] * n_inv;
    }
    std::reverse(a.begin() + 1, a.end());
    return ntt(a), a;
}

template <typename Mint, template <int...> typename NttMint>
Mint garner(int a1, int a2, int a3) {
    static constexpr auto M1 = 167772161, M2 = 469762049, M3 = 754974721;
    static constexpr auto R12 = NttMint<M2>(M1).inv().get_val();
    static constexpr auto R13 = NttMint<M3>(M1).inv().get_val();
    static constexpr auto R23 = NttMint<M3>(M2).inv().get_val();
    int x1 = a1;
    int x2 = (long long)(a2 >= x1 ? a2 - x1 : M2 + a2 - x1) * R12 % M2;
    int temp = (long long)(a3 >= x1 ? a3 - x1 : M3 + a3 - x1) * R13 % M3;
    int x3 = (long long)(temp >= x2 ? temp - x2 : M3 + temp - x2) * R23 % M3;
    return Mint(x1) + Mint(x2) * M1 + Mint(x3) * M1 * M2;
}

template <typename Mint, template <unsigned...> typename NttMint>
Mint garner(unsigned a1, unsigned a2, unsigned a3) {
    static constexpr auto M1 = 167772161u, M2 = 469762049u, M3 = 754974721u;
    static constexpr auto R12 = NttMint<M2>(M1).inv().get_val();
    static constexpr auto R13 = NttMint<M3>(M1).inv().get_val();
    static constexpr auto R23 = NttMint<M3>(M2).inv().get_val();
    unsigned x1 = a1;
    unsigned x2 = (unsigned long long)(a2 >= x1 ? a2 - x1 : M2 + a2 - x1) * R12 % M2;
    unsigned temp = (unsigned long long)(a3 >= x1 ? a3 - x1 : M3 + a3 - x1) * R13 % M3;
    unsigned x3 = (unsigned long long)(temp >= x2 ? temp - x2 : M3 + temp - x2) * R23 % M3;
    return Mint(x1) + Mint(x2) * M1 + Mint(x3) * M1 * M2;
}

template <typename Mint, template <auto...> typename NttMint>
std::vector<Mint> arbitrary_mod_convolution(std::vector<Mint> a, const std::vector<Mint> &b) {
    static constexpr decltype(NttMint<167772161>::get_m()) M1 = 167772161, M2 = 469762049, M3 = 754974721;
    auto c1 = convolution(std::vector<NttMint<M1>>(a.begin(), a.end()),
                          std::vector<NttMint<M1>>(b.begin(), b.end()));
    auto c2 = convolution(std::vector<NttMint<M2>>(a.begin(), a.end()),
                          std::vector<NttMint<M2>>(b.begin(), b.end()));
    auto c3 = convolution(std::vector<NttMint<M3>>(a.begin(), a.end()),
                          std::vector<NttMint<M3>>(b.begin(), b.end()));
    int n = (int)c1.size();
    a.resize(n);
    for (int i = 0; i < n; i++) {
        a[i] = garner<Mint, NttMint>(c1[i].get_val(), c2[i].get_val(), c3[i].get_val());
    }
    return a;
}

template <typename Mint, template <auto...> typename NttMint>
std::vector<Mint> &arbitrary_mod_inplace_convolution(std::vector<Mint> &a, const std::vector<Mint> &b) {
    static constexpr decltype(NttMint<167772161>::get_m()) M1 = 167772161, M2 = 469762049, M3 = 754974721;
    auto c1 = convolution(std::vector<NttMint<M1>>(a.begin(), a.end()),
                          std::vector<NttMint<M1>>(b.begin(), b.end()));
    auto c2 = convolution(std::vector<NttMint<M2>>(a.begin(), a.end()),
                          std::vector<NttMint<M2>>(b.begin(), b.end()));
    auto c3 = convolution(std::vector<NttMint<M3>>(a.begin(), a.end()),
                          std::vector<NttMint<M3>>(b.begin(), b.end()));
    int n = (int)c1.size();
    a.resize(n);
    for (int i = 0; i < n; i++) {
        a[i] = garner<Mint, NttMint>(c1[i].get_val(), c2[i].get_val(), c3[i].get_val());
    }
    return a;
}

template <typename T>
void normalize(std::vector<T> &a) {
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        if (a[i]) {
            a.resize(i + 1);
            return;
        }
    }
    a.clear();
}

} // namespace ntt

} // namespace polynomials
