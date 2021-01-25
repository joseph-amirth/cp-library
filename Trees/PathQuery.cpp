#include "../Graphs/GraphW.h"
#include <functional>

template<typename T>
struct path_query { //queries on edges
	using F = std::function<T(const T&, const T&)>;

	int timer, lg;
	std::vector<int> tin, tout;
	std::vector<std::vector<int>> up;
	std::vector<std::vector<T>> table;
	T e;
	F f;

	template <typename U>
	path_query(const graph<U> &g, T e, F f) : tin(g.n), tout(g.n), e(e), f(f) {
		lg = std::max(32 - __builtin_clz(g.n) - 1, 1);
		up.assign(g.n, std::vector<int>(lg, -1));
		table.assign(g.n, std::vector<int>(lg, e));
		timer = 0;
		dfs(0, -1, 0, g);
	}

	template <typename U>
	void dfs(int x, int p, int d, const graph<U> &g) {
		up[x][0] = p;
		for (int i = 1; i < lg && up[x][i - 1] != -1; i++) {
			up[x][i] = up[up[x][i - 1]][i - 1];
			table[x][i] = f(table[x][i - 1], table[up[x][i - 1]][i - 1]);
		}
		tin[x] = timer++;
		for (int i : g.adj[x]) {
			i = g.edges[i].u ^ g.edges[i].v ^ x;
			if (i == p) continue;
			table[i][0] = T(g.edges[i].w);
			dfs(i, x, d + 1, g);
		}
		tout[x] = timer++;
	}

	inline bool anc(int u, int v) {
		return tin[u] <= tin[v] and tout[u] >= tout[v];
	}

	T query(int u, int v) {
		T ans = e;
		int l = u;
		for (int i = lg - 1; i >= 0; i--) {
			if (up[l][i] != -1 && !anc(up[l][i], v)) {
				ans = f(ans, table[l][i]);
				l = up[l][i];
			}
		}
		if (!f(u, v))
			ans = f(ans, table[l][0]);
		l = v;
		for (int i = lg - 1; i >= 0; i--) {
			if (up[l][i] != -1 && !anc(up[l][i], u)) {
				ans = f(ans, table[l][i]);
				l = up[l][i];
			}
		}
		if (!f(v, u))
			ans = f(ans, table[l][0]);
		return ans;
	}
};