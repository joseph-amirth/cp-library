#include "../Graphs/Graph.h"
#include <functional>

template <typename T, typename S>
struct subtree_query { //vertex queries
	using F = std::function<T(const T&, const T&)>;

	int timer = 0;
	std::vector<int> euler, tin, tout;
	T e;
	F f;
	S st;

	template <typename U>
	subtree_query(const graph &g, const U &a, int root, T e, F f) : tin(g.n), tout(g.n), e(e), f(f) {
		euler.reserve(2 * g.n - 1);
		dfs(root, -1, g);
		std::vector<T> values(2 * g.n - 1, e);
		for (int i = 0; i < g.n; i++) values[tin[i]] = T(a[i]);
		st = S(values, 2 * g.n - 1, e, f);
	}

	void dfs(int u, int p, const graph &g) {
		tin[u] = timer++;
		euler.push_back(u);
		for (int i : g.adj[u]) {
			int v = g.edges[i].u ^ g.edges[i].v ^ u;
			if (v != p) {
				dfs(v, u, g);
				euler.push_back(u);
			}
		}
		tout[u] = timer++;
	}

	T query(int x) {
		return st.query(tin[x], tout[x]);
	}

	template <typename U>
	void update(int x, const U &val) {
		st.update(tin[x], val);
	}

	template <typename U>
	void update_subtree(int x, const U &val) {
		st.update(tin[x], tout[x], val);
	}
};