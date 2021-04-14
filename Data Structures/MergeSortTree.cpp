#include <vector>
#include <algorithm>

template <typename T>
struct merge_sort_tree {
	using node = std::vector<T>;

	int n;
	std::vector<node> t;

	merge_sort_tree() : n() {}

	template<typename U>
	merge_sort_tree(const U &arr, int n): n(n), t(4 * n) {
		build(arr, 1, 0, n - 1);
	}

	template<typename U>
	void build(const U &arr, int i, int l, int r) {
		if (l == r) {
			t[i] = {T(arr[l])};
			return;
		}
		int mid = (l + r) >> 1;
		build(arr, i << 1, l, mid);
		build(arr, i << 1 | 1, mid + 1, r);
		t[i].resize(t[i << 1].size() + t[i << 1 | 1].size());
		std::merge(t[i << 1].begin(), t[i << 1].end(), t[i << 1 | 1].begin(), t[i << 1 | 1].end(), t[i].begin());
	}

	int count(int ql, int qr, const T &x) {
		return count_helper(ql, qr, x, 1, 0, n - 1);
	}

	int count_helper(int ql, int qr, const T &x, int i, int l, int r) {
		if (r < ql || qr < l)
			return n;
		if (ql <= l && r <= qr) {
			auto [lb, ub] = std::equal_range(t[i].begin(), t[i].end(), x);
			return ub - lb;
		}
		int mid = (l + r) >> 1;
		return count_helper(ql, qr, x, i << 1, l, mid) + count_helper(ql, qr, x, i << 1 | 1, mid + 1, r);
	}
};