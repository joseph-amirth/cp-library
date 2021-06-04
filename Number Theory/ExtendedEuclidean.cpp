#include <vector>
#include <tuple>
#include "../Miscellaneous/TypePromotion.h"

namespace extended_euclidean {
	using big_int = long long;

	template<typename T>
	std::tuple<T, big_int, big_int> ext_euclid(T a, T b) {
		if (b == 0)
			return std::make_tuple(a, 1, 0);
		auto[g, x, y] = ext_euclid(b, a % b);
		return std::make_tuple(g, y, x - y * (a / b));
	}

	template<typename T>
	T gcd(T a, T b) {
		return get<0>(ext_euclid(a, b));
	}

	template<typename T>
	T mod_inverse(T a, T m) {
		auto[g, x, y] = ext_euclid(a, m);
		if (g != 1)
			return -1;
		return ((x %= m) < 0 ? x + m : x);
	}

	big_int crt(big_int a1, big_int n1, big_int a2, big_int n2) {
		auto[g, m1, m2] = ext_euclid(n1, n2);
		if (g != 1)
			return -1;
		big_int x = (a1 * m2 * n2 + a2 * m1 * n1) % (n1 * n2);
		return x < 0 ? x + n1 * n2 : x;
	}

	template <typename T>
	big_int crt(std::vector<T> a, std::vector<T> n) {
		big_int x = crt(a[0], n[0], a[1], n[1]), N = n[0] * n[1];
		for (int i = 2; i < n.size(); i++) {
			x = crt(x, N, a[i], n[i]);
			N *= n[i];
		}
		return x;
	}

	template <typename T>
	std::pair<T, T> solve_linear_congruence(T a, T b, T n) {
		T d = gcd(a, n);
		if (b % d != 0)
			return std::make_pair(-1, -1);
		a /= d, b /= d, n /= d;
		T sol = (big_int) mod_inverse(a, n) * b % n;
		return make_pair(sol, d);
	}
}