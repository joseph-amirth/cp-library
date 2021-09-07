#include <vector>
#include <algorithm>
#include "../Numeric/StaticMint.h"

namespace ntt {
	template<int P>
	struct prime_info {
		constexpr static int root = 0, root_pw = 0;
	};

	template<>
	struct prime_info<7340033> {
		constexpr static int root = 5, root_pw = 1 << 20;
	};

	template<>
	struct prime_info<998244353> {
		constexpr static int root = 15311432, root_pw = 1 << 23;
	};

	template<>
	struct prime_info<754974721> {
		constexpr static int root = 739831874, root_pw = 1 << 24;
	};

	template<>
	struct prime_info<167772161> {
		constexpr static int root = 243, root_pw = 1 << 25;
	};

	template<>
	struct prime_info<469762049> {
		constexpr static int root = 2187, root_pw = 1 << 26;
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

	template<int M>
	std::vector<static_mint<M>> root = {0, 1};

	template<int M>
	void compute_roots(int lg) {
		static int computed = 1;
		if (lg <= computed) return;
		root<M>.resize(1 << lg);
		for (int k = computed; k < lg; k++) {
			static_mint<M> z(prime_info<M>::root);
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

	template<int M>
	void ntt(std::vector<static_mint<M>> &a) {
		int n = int(a.size()), lg = 32 - __builtin_clz(n) - 1;
		compute_bit_reverse(lg);
		compute_roots<M>(lg);
		int shift = 31 - lg;
		for (int i = 0; i < n; i++) {
			if (i < (rev[i] >> shift)) {
				std::swap(a[i], a[rev[i] >> shift]);
			}
		}
		for (int k = 1; k < n; k <<= 1) {
			for (int i = 0; i < n; i += 2 * k) {
				for (int j = 0; j < k; j++) {
					static_mint<M> z = root<M>[j + k] * a[i + j + k];
					a[i + j + k] = a[i + j] - z;
					a[i + j] = a[i + j] + z;
				}
			}
		}
	}

	template<int M>
	std::enable_if_t<prime_info<M>::root != 0, std::vector<static_mint<M>>>
	convolution(std::vector<static_mint<M>> a, std::vector<static_mint<M>> b) {
		int n = 1;
		while (n < a.size() + b.size()) {
			n <<= 1;
		}
		a.resize(n), b.resize(n);
		ntt(a), ntt(b);
		static_mint<M> n_inv = static_mint<M>(n).inv();
		for (int i = 0; i < n; i++) {
			a[i] *= b[i] * n_inv;
		}
		std::reverse(a.begin() + 1, a.end());
		return ntt(a), a;
	}

	template <int M>
	std::enable_if_t<prime_info<M>::root != 0, void>
	inplace_convolution(std::vector<static_mint<M>> &a, std::vector<static_mint<M>> b) {
		int n = 1;
		while (n < a.size() + b.size()) {
			n <<= 1;
		}
		a.resize(n), b.resize(n);
		ntt(a), ntt(b);
		static_mint<M> n_inv = static_mint<M>(n).inv();
		for (int i = 0; i < n; i++) {
			a[i] *= b[i] * n_inv;
		}
		std::reverse(a.begin() + 1, a.end());
		ntt(a);
	}

	template<int M>
	static_mint<M> garner(int a1, int a2, int a3) {
		constexpr auto M1 = 754974721, M2 = 167772161, M3 = 469762049;
		constexpr auto R12 = static_mint<M2>(M1).inv().val;
		constexpr auto R13 = static_mint<M3>(M1).inv().val;
		constexpr auto R23 = static_mint<M3>(M2).inv().val;
		int x1 = a1;
		int x2 = (long long) (a2 - x1) * R12 % M2;
		if (x2 < 0) x2 += M2;
		int x3 = ((long long) (a3 - x1) * R13 % M3 - x2) * R23 % M3;
		if (x3 < 0) x3 += M3;
		return static_mint<M>(x1) + static_mint<M>(x2) * M1 + static_mint<M>(x3) * M1 * M2;
	}

	template<int M>
	std::enable_if_t<prime_info<M>::root == 0, std::vector<static_mint<M>>>
	convolution(std::vector<static_mint<M>> a, const std::vector<static_mint<M>> &b) {
		constexpr auto M1 = 754974721, M2 = 167772161, M3 = 469762049;
		auto c1 = convolution(std::vector<static_mint<M1>>(a.begin(), a.end()), std::vector<static_mint<M1>>(b.begin(), b.end()));
		auto c2 = convolution(std::vector<static_mint<M2>>(a.begin(), a.end()), std::vector<static_mint<M2>>(b.begin(), b.end()));
		auto c3 = convolution(std::vector<static_mint<M3>>(a.begin(), a.end()), std::vector<static_mint<M3>>(b.begin(), b.end()));
		int n = (int) c1.size();
		a.resize(n);
		for (int i = 0; i < n; i++) {
			a[i] = garner<M>(c1[i].val, c2[i].val, c3[i].val);
		}
		return a;
	}

	template<int M = 998244353, typename T>
	std::enable_if_t<!is_mint<T>::value, std::vector<static_mint<M>>>
	convolution(const std::vector<T> &a, const std::vector<T> &b) {
		return convolution(std::vector<static_mint<M>>(a.begin(), a.end()), std::vector<static_mint<M>>(b.begin(), b.end()));
	}

	int garner(int a1, int a2, int a3, int M) {
		constexpr auto M1 = 754974721, M2 = 167772161, M3 = 469762049;
		constexpr auto R12 = static_mint<M2>(M1).inv().val;
		constexpr auto R13 = static_mint<M3>(M1).inv().val;
		constexpr auto R23 = static_mint<M3>(M2).inv().val;
		int x1 = a1;
		int x2 = (long long) (a2 - x1) * R12 % M2;
		if (x2 < 0) x2 += M2;
		int x3 = ((long long) (a3 - x1) * R13 % M3 - x2) * R23 % M3;
		if (x3 < 0) x3 += M3;
		return (x1 + (long long)x2 * M1 + (long long)x3 * M1 % M * M2) % M;
	}

	template <typename T>
	std::enable_if_t<!is_mint<T>::value, std::vector<T>>
	convolution(const std::vector<T> &a, const std::vector<T> &b, int M) {
		constexpr auto M1 = 754974721, M2 = 167772161, M3 = 469762049;
		auto c1 = convolution(std::vector<static_mint<M1>>(a.begin(), a.end()), std::vector<static_mint<M1>>(b.begin(), b.end()));
		auto c2 = convolution(std::vector<static_mint<M2>>(a.begin(), a.end()), std::vector<static_mint<M2>>(b.begin(), b.end()));
		auto c3 = convolution(std::vector<static_mint<M3>>(a.begin(), a.end()), std::vector<static_mint<M3>>(b.begin(), b.end()));
		int n = (int) c1.size();
		a.resize(n);
		for (int i = 0; i < n; i++) {
			a[i] = garner(c1[i].val, c2[i].val, c3[i].val, M);
		}
		return a;
	}

	template<typename T>
	void normalize(const std::vector<T> &a) {
		for (int i = int(a.size()) - 1; i >= 0; i--) {
			if (a[i]) {
				a.resize(i + 1);
				return;
			}
		}
		a.clear();
	}
}