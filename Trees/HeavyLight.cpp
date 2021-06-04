#include "LCA.cpp"

template<typename T, typename S>
struct heavy_light : public LCA {
	using F = std::function<T(const T&, const T&)>;

	std::vector<int> sz, tail, head;
	std::vector<int> tree_pos;
	int nxt = 0;
	S st;
	T e;
	F f;

	template<typename U>
	heavy_light(const graph &g, const std::vector<U> &a, T e, F f) : LCA(g), e(e), f(f) {
		sz.assign(g.n, 1);
		tail.assign(g.n, 0);
		head.assign(g.n, 0);
		tree_pos.assign(g.n, 0);
		decompose(0, g);
		build(0);
		std::vector<T> values(g.n);
		for (int i = 0; i < g.n; i++) {
			values[tree_pos[i]] = T(a[i]);
		}
		st = S(values, g.n, e, f);
	}

	void decompose(int u, const graph &g) {
		tail[u] = u;
		int mx = -1;
		for (int i : g.adj[u]) {
			int v = g.edges[i].u ^ g.edges[i].v ^ u;
			if (v == up[u][0])
				continue;
			decompose(v, g);
			sz[u] += sz[v];
			if (mx == -1)
				mx = v;
			else if (sz[mx] <= sz[v])
				build(mx), mx = v;
			else build(v);
		}
		if (mx != -1 && 2 * sz[mx] >= sz[u])
			tail[u] = tail[mx];
		else if (mx != -1) build(mx);
	}

	void build(int x) {
		for (int i = tail[x]; i != up[x][0]; i = up[i][0], nxt++) {
			tree_pos[i] = nxt;
			head[i] = x;
		}
	}

	T query(int u, int v) {
		int l = lca(u, v);
		if (l == u)
			return query_helper(v, lvl[u]);
		if (l == v)
			return query_helper(u, lvl[v]);
		return f(query_helper(u, lvl[l] + 1), query_helper(v, lvl[l]));
	}

	T query_helper(int x, int k) {
		T ans = e;
		while (lvl[head[x]] > k) {
			ans = f(ans, st.query(tree_pos[x], tree_pos[head[x]]));
			x = up[head[x]][0];
		}
		ans = f(ans, st.query(tree_pos[x], tree_pos[x] + lvl[x] - k));
		return ans;
	}

	template<typename U>
	void update(int x, const U &val) {
		st.update(tree_pos[x], val);
	}

	template<typename U>
	void update(int u, int v, const U &val) {
		int l = lca(u, v);
		if (l == u)
			update_helper(v, lvl[l], val);
		else if (l == v)
			update_helper(u, lvl[l], val);
		else update_helper(u, lvl[l] + 1, val), update_helper(v, lvl[l], val);
	}

	template<typename U>
	void update_helper(int x, int k, const U &val) {
		while (lvl[head[x]] > k) {
			st.update(tree_pos[x], tree_pos[head[x]], val);
			x = up[head[x]][0];
		}
		st.update(tree_pos[x], tree_pos[x] + lvl[x] - k, val);
	}
};