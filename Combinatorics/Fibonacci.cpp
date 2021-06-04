#include "../Numeric/Matrix.h"

namespace fibonacci {
	template <typename T>
	T fib(long long n) {
		matrix<T> a(2, 2);
		a[0][0] = a[0][1] = 1;
		a[1][0] = 1, a[1][1] = 0;

		matrix<T> ans(2, 2);
		ans[0][0] = ans[1][1] = 1;

		if (n == 0) {
			return 0;
		} else {
			for (n -= 1; n; n /= 2) {
				if (n & 1) ans *= a;
				a *= a;
			}
			return ans[0][0];
		}
	}

	template <typename T>
	T sum(long long n) {
		return fib<T>(n + 2) - 1;
	}

	template <typename T>
	T fib(T a, T b, long long n) {
		return fib<T>(n + 1) * a + (fib<T>(n + 2) - 1) * b;
	}

	template <typename T>
	T sum(T a, T b, long long n) {
		if (n == 0) {
			return a;
		} else {
			return fib<T>(n - 1) * a + fib<T>(n) * b;
		}
	}
}