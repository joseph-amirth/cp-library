#include <vector>
#include <functional>

template <typename T>
struct disjoint_sparse_table {
	using F = std::function<T(const T&, const T&)>;

	int n, lg;
	std::vector<std::vector<T>> table;
	F f;
	disjoint_sparse_table() : n(), f() { }

	template <typename U>
	disjoint_sparse_table(const U &a, int n, F f) : n(n), f(f) {
		lg = 32 - __builtin_clz(n) - 1;
		if (n == 1 || n != 1 << lg) lg += 1;
		table.assign(lg, std::vector<T>(std::begin(a), std::begin(a) + n));
		for (int k = 1; k < lg; k++) {
			for (int j = 1 << k; j < n; j += 2 << k) {
				for (int i = j - 2; i >= j - (1 << k); i--) {
					table[k][i] = f(a[i], table[k][i + 1]);
				}
				for (int i = j + 1; i < std::min(n, j + (1 << k)); i++) {
					table[k][i] = f(table[k][i - 1], a[i]);
				}
			}
		}
	}

	T query(int l, int r) {
		if (l != r) {
			int j = 32 - __builtin_clz(l ^ r) - 1;
			return f(table[j][l], table[j][r]);
		} else {
			return table[0][l];
		}
	}
};