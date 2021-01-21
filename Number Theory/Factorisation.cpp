#include <vector>

template<typename T>
bool is_prime(T x) {
	if (x == 1) {
		return false;
	}
	for (int i = 2; (T) i * i <= x; i++) {
		if (x % i == 0) {
			return false;
		}
	}
	return true;
}

template<typename T>
std::vector<std::pair<T, int>> prime_factors(T n) {
	std::vector<std::pair<T, int>> factors;
	for (int i = 2; (T) i * i <= n; i++) {
		if (n % i == 0) {
			factors.emplace_back(i, 0);
			while (n % i == 0) {
				n /= i;
				factors.back().second++;
			}
		}
	}
	if (n > 1) {
		factors.emplace_back(n, 1);
	}
	return factors;
}

template<typename T>
std::vector<T> divisors(T n) {
	std::vector<T> divisors;
	for (int i = 1; (T) i * i <= n; i++) {
		if (n % i == 0) {
			divisors.push_back(i);
			if (i != n / i) {
				divisors.push_back(n / i);
			}
		}
	}
	return divisors;
}

template <typename T>
T totient(T n) {
	auto factors = prime_factors(n);
	T ans = n;
	for (auto&[p, c] : factors) {
		ans /= p;
		ans *= (p - 1);
	}
	return ans;
}