#pragma once

#include <vector>
#include <immintrin.h>
#include <algorithm>
#include "../mint/montgomery-mint.hpp"

namespace ntt {
    template<uint32_t P>
    struct prime_info {
        constexpr static uint32_t root = 0, root_pw = 0;
    };

    template<>
    struct prime_info<7340033> {
        constexpr static uint32_t root = 5, root_pw = 1 << 20;
    };

    template<>
    struct prime_info<998244353> {
        constexpr static uint32_t root = 15311432, root_pw = 1 << 23;
    };

    template<>
    struct prime_info<754974721> {
        constexpr static uint32_t root = 739831874, root_pw = 1 << 24;
    };

    template<>
    struct prime_info<167772161> {
        constexpr static uint32_t root = 243, root_pw = 1 << 25;
    };

    template<>
    struct prime_info<469762049> {
        constexpr static uint32_t root = 2187, root_pw = 1 << 26;
    };

    std::vector<int> rev = {0};

    void compute_bit_reverse(int lg) {
        static int computed = 0;
        if (lg <= computed) return;
        rev.resize(1 << lg);
        for (int i = (1 << computed); i < (1 << lg); i++) {
            rev[i] = (rev[i >> 1] >> 1) ^ ((i & 1) << 30);
        }
        computed = lg;
    }

    template<uint32_t M>
    std::vector<montgomery_mint<M>> root = {0, 1};

    template<uint32_t M>
    void compute_roots(int lg) {
        static int computed = 1;
        if (lg <= computed) return;
        root<M>.resize(1 << lg);
        for (int k = computed; k < lg; k++) {
            montgomery_mint<M> z(prime_info<M>::root);
            for (int i = (1 << (k + 1)); i < prime_info<M>::root_pw; i <<= 1) {
                z *= z;
            }
            for (int i = (1 << (k - 1)); i < (1 << k); i++) {
                root<M>[i << 1] = root<M>[i];
                root<M>[i << 1 | 1] = root<M>[i] * z;
            }
        }
        computed = lg;
    }

    template<uint32_t M>
    void ntt(std::vector<montgomery_mint<M>> &_a) {
        int n = (int) _a.size(), *a = _a.data();
        int lg = 32 - __builtin_clz(n) - 1;
        compute_bit_reverse(lg), compute_roots<M>(lg);
        int shift = 31 - lg;
        for (int i = 0; i < n; i++) {
            if (i < (rev[i] >> shift)) {
                std::swap(a[i], a[rev[i] >> shift]);
            }
        }

        const static __m256i __M = _mm256_set1_epi32(M);
        const static __m256i __M_1 = _mm256_set1_epi32(M - 1);
        const static __m256i __0 = _mm256_set1_epi32(0);

        const static __m256i __M64 = _mm256_set1_epi64x(M);
        const static __m256i __M_1_64 = _mm256_set1_epi64x(M - 1);
        const static __m256i __M_dash = _mm256_set1_epi64x(montgomery_mint<M>::get_m_dash());

        for (int k = 1; k < n; k <<= 1) {
//			if (k == 1) {
//				__m256i __a, __b, __add, __sub, __res0, __res1;
//				const static __m256i __shuffle_mask = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);
//				for (int i = 0; i < n; i += 16) {
//					__a = _mm256_loadu_si256((__m256i * )(a + i));
//					__b = _mm256_loadu_si256((__m256i * )(a + i + 8));
//
//					__add = _mm256_permute4x64_epi64(_mm256_hadd_epi32(__a, __b), 0xd8);
//					__sub = _mm256_permute4x64_epi64(_mm256_hsub_epi32(__a, __b), 0xd8);
//
//					__add = _mm256_sub_epi32(__add, _mm256_and_si256(__M, _mm256_cmpgt_epi32(__add, __M_1)));
//					__sub = _mm256_add_epi32(__sub, _mm256_and_si256(__M, _mm256_cmpgt_epi32(__0, __sub)));
//
//					__res0 = _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(__add, __sub, 0b0010'0000),
//														 __shuffle_mask);
//					__res1 = _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(__add, __sub, 0b0011'0001),
//														 __shuffle_mask);
//
//					_mm256_storeu_si256((__m256i * )(a + i), __res0);
//					_mm256_storeu_si256((__m256i * )(a + i + 8), __res1);
//				}
//			} else if (k == 2) {
//				for (int i = 0; i < n; i += 2 * k) {
//					for (int j = 0; j < k; j++) {
//
//					}
//				}
//			} else if (k == 4) {
//				for (int i = 0; i < n; i += 2 * k) {
//					for (int j = 0; j < k; j += 4) {
//						__m256i _a = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i *) &root<M>[j + k]));
//						__m256i _b = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i *) a + i + j));
//						__m256i _c = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i *) a + i + j + k));
//						_a = _mm256_montgomery_mul<M>(_a, _c);
//
//						_c = _mm256_sub_epi64(_b, _a);
//						_b = _mm256_add_epi64(_b, _a);
//
//
//					}
//				}
//
//			} else {
//				for (int i = 0; i < n; i += 2 * k) {
//					for (int j = 0; j < k; j += 8) {
//
//					}
//				}
//			}
            if (k < 8) {
                for (int i = 0; i < n; i += 2 * k) {
                    for (int j = 0; j < k; j++) {
                        auto z = montgomery_mint<M>::reduce((uint64_t) root<M>[j + k].val * a[i + j + k]);
                        a[i + j + k] = a[i + j] - z;
                        if (a[i + j + k] >= M) {
                            a[i + j + k] += M;
                        }
                        a[i + j] = a[i + j] + z;
                        if (a[i + j] >= M) {
                            a[i + j] -= M;
                        }
                    }
                }
            } else {
                for (int i = 0; i < n; i += 2 * k) {
                    for (int j = 0; j < k; j += 8) {
                        __m256i _a = _mm256_cvtepu32_epi64(
                                _mm_loadu_si128((__m128i *) ((uint32_t *) root<M>.data() + j + k)));
                        __m256i _c = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i *) (a + i + j + k)));

                        _a = _c = _mm256_mul_epu32(_a, _c);
                        _a = _mm256_mul_epu32(_a, __M_dash);
                        _a = _mm256_mul_epu32(_a, __M64);
                        _a = _mm256_add_epi64(_a, _c);
                        _a = _mm256_srli_epi64(_a, 32);
                        _a = _mm256_sub_epi64(_a, _mm256_and_si256(__M64, _mm256_cmpgt_epi64(_a, __M_1_64)));

                        __m256i _b = _mm256_cvtepu32_epi64(
                                _mm_loadu_si128((__m128i *) ((uint32_t *) root<M>.data() + j + k + 4)));
                        __m256i _d = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i *) (a + i + j + k + 4)));

                        _b = _d = _mm256_mul_epu32(_b, _d);
                        _b = _mm256_mul_epu32(_b, __M_dash);
                        _b = _mm256_mul_epu32(_b, __M64);
                        _b = _mm256_add_epi64(_b, _d);
                        _b = _mm256_srli_epi64(_b, 32);
                        _b = _mm256_sub_epi64(_b, _mm256_and_si256(__M64, _mm256_cmpgt_epi64(_b, __M_1_64)));

                        __m256i _final = _mm256_blend_epi32(_a, _mm256_bslli_epi128(_b, 4), 0b10101010);
                        _final = _mm256_permutevar8x32_epi32(_final, _mm256_set_epi32(7, 5, 3, 1, 6, 4, 2, 0));

                        __m256i _e = _mm256_loadu_si256((__m256i *) (a + i + j));
                        __m256i _f = _mm256_loadu_si256((__m256i *) (a + i + j));
                        _e = _mm256_add_epi32(_e, _final);
                        _e = _mm256_sub_epi32(_e, _mm256_and_si256(__M, _mm256_or_si256(_mm256_cmpgt_epi32(_e, __M_1),
                                                                                        _mm256_cmpgt_epi32(__0, _e))));
                        _mm256_storeu_si256((__m256i *) (a + i + j), _e);

                        _f = _mm256_sub_epi32(_f, _final);
                        _f = _mm256_add_epi32(_f, _mm256_and_si256(__M, _mm256_cmpgt_epi32(__0, _f)));
                        _mm256_storeu_si256((__m256i *) (a + i + j + k), _f);
                    }
                }
            }
        }
    }

    template<typename mint>
    std::vector<mint> naive_convolution(const std::vector<mint> &a, const std::vector<mint> &b) {
        std::vector<mint> c(a.size() + b.size() - 1);
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) b.size(); j++) {
                c[i + j] += a[i] * b[j];
            }
        }
        return c;
    }

    template <uint32_t M>
    std::enable_if_t<prime_info<M>::root != 0, std::vector<montgomery_mint<M>>>
    convolution(std::vector<montgomery_mint<M>> a, std::vector<montgomery_mint<M>> b) {
        int n = 1;
        while (n < a.size() + b.size()) {
            n <<= 1;
        }
        a.resize(n), b.resize(n);
        ntt(a), ntt(b);
        montgomery_mint<M> n_inv = montgomery_mint<M>(n).inv();
        for (int i = 0; i < n; i++) {
            a[i] *= b[i] * n_inv;
        }
        std::reverse(a.begin() + 1, a.end());
        return ntt(a), a;
    }

    template <uint32_t M>
    std::enable_if_t<prime_info<M>::root != 0, std::vector<montgomery_mint<M>>&>
    inplace_convolution(std::vector<montgomery_mint<M>> &a, std::vector<montgomery_mint<M>> b) {
        int n = 1;
        while (n < a.size() + b.size()) {
            n <<= 1;
        }
        a.resize(n), b.resize(n);
        ntt(a), ntt(b);
        montgomery_mint<M> n_inv = montgomery_mint<M>(n).inv();
        for (int i = 0; i < n; i++) {
            a[i] *= b[i] * n_inv;
        }
        std::reverse(a.begin() + 1, a.end());
        return ntt(a), a;
    }

    template<typename mint>
    mint garner(uint32_t a1, uint32_t a2, uint32_t a3) {
        static constexpr auto M1 = 167772161u, M2 = 469762049u, M3 = 754974721u;
        static constexpr auto R12 = montgomery_mint<M2>(M1).inv().get_val();
        static constexpr auto R13 = montgomery_mint<M3>(M1).inv().get_val();
        static constexpr auto R23 = montgomery_mint<M3>(M2).inv().get_val();
        uint32_t x1 = a1;
        uint32_t x2 = (uint64_t) (a2 >= x1 ? a2 - x1 : M2 + a2 - x1) * R12 % M2;
        uint32_t temp = (uint64_t) (a3 >= x1 ? a3 - x1 : M3 + a3 - x1) * R13 % M3;
        uint32_t x3 = (uint64_t) (temp >= x2 ? temp - x2 : M3 + temp - x2) * R23 % M3;
        return mint(x1) + mint(x2) * M1 + mint(x3) * M1 * M2;
    }

    template<typename mint>
    std::vector<mint> arbitrary_mod_convolution(std::vector<mint> a, const std::vector<mint> &b) {
        static constexpr auto M1 = 167772161u, M2 = 469762049u, M3 = 754974721u;
        auto c1 = convolution(std::vector<montgomery_mint<M1>>(a.begin(), a.end()),
                              std::vector<montgomery_mint<M1>>(b.begin(), b.end()));
        auto c2 = convolution(std::vector<montgomery_mint<M2>>(a.begin(), a.end()),
                              std::vector<montgomery_mint<M2>>(b.begin(), b.end()));
        auto c3 = convolution(std::vector<montgomery_mint<M3>>(a.begin(), a.end()),
                              std::vector<montgomery_mint<M3>>(b.begin(), b.end()));
        int n = (int) c1.size();
        a.resize(n);
        for (int i = 0; i < n; i++) {
            a[i] = garner<mint, montgomery_mint>(c1[i].get_val(), c2[i].get_val(), c3[i].get_val());
        }
        return a;
    }

    template<typename mint>
    std::vector<mint> &arbitrary_mod_inplace_convolution(std::vector<mint> &a, const std::vector<mint> &b) {
        static constexpr auto M1 = 167772161, M2 = 469762049, M3 = 754974721;
        auto c1 = convolution(std::vector<montgomery_mint<M1>>(a.begin(), a.end()),
                              std::vector<montgomery_mint<M1>>(b.begin(), b.end()));
        auto c2 = convolution(std::vector<montgomery_mint<M2>>(a.begin(), a.end()),
                              std::vector<montgomery_mint<M2>>(b.begin(), b.end()));
        auto c3 = convolution(std::vector<montgomery_mint<M3>>(a.begin(), a.end()),
                              std::vector<montgomery_mint<M3>>(b.begin(), b.end()));
        int n = (int) c1.size();
        a.resize(n);
        for (int i = 0; i < n; i++) {
            a[i] = garner<mint>(c1[i].get_val(), c2[i].get_val(), c3[i].get_val());
        }
        return a;
    }

    template<typename T>
    void normalize(const std::vector<T> &a) {
        for (int i = (int) a.size() - 1; i >= 0; i--) {
            if (a[i]) {
                a.resize(i + 1);
                return;
            }
        }
        a.clear();
    }
}