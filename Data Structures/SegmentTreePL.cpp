#include <vector>
#include <functional>

template<typename T>
struct segment_tree {
	using F = std::function<T(const T&, const T&)>;

	struct node {
		T val;
		int lazy, l, r;
		node(T val) : val(val), lazy() {}
		node(T val, int l, int r) : val(val), lazy(), l(l), r(r) {}
		node(T val, int lazy, int l, int r): val(val), lazy(lazy), l(l), r(r) {}
	};

	std::vector<node> t;

	template <typename...args>
	int new_node(args...A) {
		int id = (int) t.size();
		t.emplace_back(A...);
		return id;
	}

	int n, cur_ver;
	std::vector<int> ver;
	T e;
	F f;

	segment_tree() : ver(), n(), cur_ver(), e(), f() {}

	void change_ver(int k) {
		cur_ver = ver[k];
	}

	template<typename U>
	segment_tree(const U &arr, int n, T e, F f): n(n), e(e), f(f) {
		ver.push_back(build(arr, 0, n - 1));
		cur_ver = ver.back();
	}

	template<typename U>
	int build(const U &arr, int l, int r) {
		if (l == r)
			return new_node(T(arr[l]));
		int mid = (l + r) >> 1;
		int vl = build(arr, l, mid);
		int vr = build(arr, mid + 1, r);
		return new_node(f(t[vl].val, t[vr].val), vl, vr);
	}

	void push(int i, int l, int r) {
		//lazy
	}

	template<typename U>
	int update(int idx, U val) {
		ver.push_back(update_helper(idx, val, cur_ver, 0, n - 1));
		cur_ver = ver.back();
		return ver.back();
	}

	template<typename U>
	int update_helper(int idx, U val, int v, int l, int r) {
		if (l == r)
			return new_node(T(val));
		push(v, l, r);
		int mid = (l + r) >> 1;
		if (idx <= mid) {
			int vl = update_helper(idx, val, t[v].l, l, mid);
			return new_node(f(t[vl].val, t[t[v].r].val), vl, t[v].r);
		} else {
			int vr = update_helper(idx, val, t[v].r, mid + 1, r);
			return new_node(f(t[t[v].l].val, t[vr].val), t[v].l, vr);
		}
	}

	template <typename U>
	int update(int ql, int qr, U val) {
		ver.push_back(update_helper(ql, qr, val, cur_ver, 0, n - 1));
		cur_ver = ver.back();
		return ver.back();
	}

	template <typename U>
	int update_helper(int ql, int qr, U val, int v, int l, int r) {
		if (r < ql || qr < l)
			return v;
		if (ql <= l && r <= qr)
			;//lazy
		push(v, l, r);
		int mid = (l + r) >> 1;
		int vl = update_helper(ql, qr, val, t[v].l, l, mid);
		int vr = update_helper(ql, qr, val, t[v].r, mid + 1, r);
		return new_node(f(t[vl].val, t[vr].val), vl, vr);
	}

	T query(int ql, int qr, int k) {
		return query_helper(ql, qr, ver[k], 0, n - 1);
	}

	T query(int ql, int qr) {
		return query_helper(ql, qr, cur_ver, 0, n - 1);
	}

	T query_helper(int ql, int qr, int v, int l, int r) {
		if (r < ql || qr < l)
			return e;
		if (ql <= l && r <= qr)
			return t[v].val;
		push(v, l, r);
		int mid = (l + r) >> 1;
		return f(query_helper(ql, qr, t[v].l, l, mid), query_helper(ql, qr, t[v].r, mid + 1, r));
	}
};