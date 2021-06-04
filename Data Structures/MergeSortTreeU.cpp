#include <set>
#include <vector>
#include <algorithm>

template <typename T>
struct merge_sort_tree {
	using node = std::multiset<T>;

	int n;
	std::vector<node> t;

	merge_sort_tree() : n() {}

	template<typename U>
	merge_sort_tree(const std::vector<U> &v): n(v.size()), t(4 * n) {
		build(v, 1, 0, n - 1);
	}

	template<typename U>
	void build(const std::vector<U> &v, int i, int l, int r) {
		if (l == r) {
			t[i] = {T(v[l])};
			return;
		}
		int mid = (l + r) >> 1;
		build(v, i << 1, l, mid);
		build(v, i << 1 | 1, mid + 1, r);
		std::merge(t[i << 1].begin(), t[i << 1].end(), t[i << 1 | 1].begin(), t[i << 1 | 1].end(), inserter(t[i], t[i].end()));
	}

	void update(int idx, T x) {
		update_helper(idx, x, 1, 0, n - 1);
	}

	void update_helper(int idx, T &x, int i, int l, int r) {
		if (l == r) {
			int y = *t[i].begin();
			t[i] = {x}, x = y;
			return;
		}
		int mid = (l + r) >> 1;
		t[i].insert(x);
		if (idx <= mid)
			update_helper(idx, x, i << 1, l, mid);
		else update_helper(idx, x, i << 1 | 1, mid + 1, r);
		t[i].erase(t[i].find(x));
	}
};