#include "../Graphs/Graph.h"
#include "../Data Structures/SparseTable.h"

struct LCA {
	int timer, lg;
	std::vector<int> tin, tout, lvl;
	std::vector<std::vector<int>> up;

	LCA(const graph &g, int root = 0) : timer(0), tin(g.n), tout(g.n), lvl(g.n) {
		lg = 32 - __builtin_clz(g.n);
		up.assign(g.n, std::vector<int>(lg, -1));

		auto dfs = [&](int u, const auto &self) -> void {
			tin[u] = timer++;
			for (int i = 1; i < lg && up[u][i - 1] != -1; i++) {
				up[u][i] = up[up[u][i - 1]][i - 1];
			}
			for (int i : g.adj[u]) {
				int v = g.edges[i].u ^ g.edges[i].v ^ u;
				if (v != up[u][0]) {
					lvl[v] = lvl[u] + 1;
					up[v][0] = u;
					self(v, self);
				}
			}
			tout[u] = timer++;
		};

		if (root == 0) {
			for (int u = 0; u < g.n; u++) {
				if (up[u][0] == -1) {
					dfs(u, dfs);
				}
			}
		} else {
			dfs(root, dfs);
		}
	}

	inline bool is_anc(int u, int v) {
		return tin[u] <= tin[v] && tout[u] >= tout[v];
	}

	int lca(int u, int v) {
		if (is_anc(u, v))
			return u;
		if (is_anc(v, u))
			return v;
		for (int i = lg - 1; i >= 0; i--) {
			if (up[u][i] != -1 && !is_anc(up[u][i], v)) {
				u = up[u][i];
			}
		}
		return up[u][0];
	}

	int dist(int u, int v) {
		return lvl[u] + lvl[v] - 2 * lvl[lca(u, v)];
	}
};