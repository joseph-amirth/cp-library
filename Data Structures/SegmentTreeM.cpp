#include <vector>

struct segment_tree {
	struct node {
		int val;
		node() : val(0) {}
		node(int val) : val(val) {}
		node(int ll, int rr) {
			val = (rr - ll + 1);
		}

		friend node operator+(const node &x, const node &y) {
			return node(x.val + y.val);
		}
	};

	std::vector<node> t;
	std::vector<int> left, right;

	template <typename...args>
	int new_node(args...A) {
		t.emplace_back(A...);
		left.push_back(-1);
		right.push_back(-1);
		return int(t.size()) - 1;
	}

	int n;

	segment_tree() : n() {}

	segment_tree(int n) : n(n) {
		new_node(0, n - 1);
	}

	void reserve(size_t sz) {
		t.reserve(sz);
		left.reserve(sz);
		right.reserve(sz);
	}

	template<typename U>
	void update(int idx, const U &val) {
		update_helper(idx, val, 0, 0, n - 1);
	}

	template<typename U>
	void update_helper(int idx, const U &val, int i, int l, int r) {
		if (l == r) {
			t[i].val = int(val);
			return;
		}
		int mid = (l + r) >> 1;
		if (left[i] == -1 && right[i] == -1) {
			left[i] = new_node(l, mid);
			right[i] = new_node(mid + 1, r);
		}
		if (idx <= mid)
			update_helper(idx, val, left[i], l, mid);
		else update_helper(idx, val, right[i], mid + 1, r);
		t[i] = t[left[i]] + t[right[i]];
	}

	node query(int ql, int qr) {
		return query_helper(ql, qr, 0, 0, n - 1);
	}

	node query_helper(int ql, int qr, int i, int l, int r) {
		if (r < ql || qr < l)
			return node();
		if (ql <= l && r <= qr)
			return t[i];
		int mid = (l + r) >> 1;
		if (left[i] == -1 && right[i] == -1) {
			left[i] = new_node(l, mid);
			right[i] = new_node(mid + 1, r);
		}
		node x = query_helper(ql, qr, left[i], l, mid), y = query_helper(ql, qr, right[i], mid + 1, r);
		return x + y;
	}
};