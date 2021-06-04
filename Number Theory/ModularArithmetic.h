#include <vector>
#include "../Miscellaneous/TypePromotion.h"

namespace modular_arithmetic {
	template <typename T>
	T modexp(T x, T n, T m) {
		T ans = 1;
		for (x %= m; n; n /= 2) {
			if (n & 1) ans = (promote_t<T>) ans * x % m;
			x = (promote_t<T>) x * x % m;
		}
		return ans;
	}

	template <typename T>
	T modinv(T x, T m) {
		return modexp(x, m - 2, m);
	}

	template <typename T>
	T primitive_root(T n) {
		std::vector<T> factors;
		T x = phi(n);
		for (int i = 2; (T) i * i <= x; i++) {
			if (x % i == 0) {
				factors.push_back(i);
				while (x % i == 0) {
					x /= i;
				}
			}
		}
		if (x > 1) {
			factors.push_back(x);
		}
		for (int i = 1; i < n; i++) {
			bool is_primitive_root = true;
			for (auto f : factors) {
				is_primitive_root &= modexp(i, (n - 1) / f, n) != 1;
			}
			if (is_primitive_root) {
				return i;
			}
		}
		return -1;
	}
}