#include <vector>

constexpr int mod = 1'000'000'007;

namespace modular_arithmetic {
	long long modexp(long long x, long long n, int m = mod) {
	    long long ans = 1;
	    for (x %= m; n; n /= 2) {
	        if (n & 1) ans = ans * x % m;
	        x = x * x % m;
	    }
	    return ans;
	}

	long long modinv(long long x, int m = mod) {
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