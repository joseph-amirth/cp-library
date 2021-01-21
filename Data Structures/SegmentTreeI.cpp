#include <vector>
#include <functional>

template<typename T>
struct segment_tree {
	using F = std::function<T(const T&, const T&)>;

	int n;
	std::vector<T> t;
	T e;
	F f;
	segment_tree() : n(), e(), f() {}

	template<typename U>
	segment_tree(const U &arr, int n, T e, F f): n(n), t(2 * n), e(e), f(f) {
		for (int i = 0; i < n; i++)
			t[i + n] = T(arr[i]);
		for (int i = n - 1; i > 0; i--)
			t[i] = f(t[i << 1], t[i << 1 | 1]);
	}

	template<typename U>
	void update(int i, U val) {
		for (t[i += n] = T(val); i > 1; i >>= 1) {
			t[i >> 1] = f(t[i], t[i ^ 1]);
		}
	}

	T query(int l, int r) {
		T ans = e;
		for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
			if (l & 1) ans = f(ans, t[l++]);
			if (r & 1) ans = f(ans, t[--r]);
		}
		return ans;
	}
};