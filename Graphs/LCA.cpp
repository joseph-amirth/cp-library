#include "Graph.h"
#include "../Data Structures/SparseTable.h"

struct LCA {
	int timer, lg;
	std::vector<int> tin, tout, lvl;
	std::vector<std::vector<int>> up;

	LCA(const graph &g, int root = 0) : timer(0), tin(g.n), tout(g.n), lvl(g.n) {
		lg = std::max(32 - __builtin_clz(g.n) - 1, 1);
		up.assign(g.n, std::vector<int>(lg, -1));
		dfs(root, -1, g);
	}

	void dfs(int x, int p, const graph &g) {
		tin[x] = timer++;
		up[x][0] = p;
		for (int i = 1; i < lg && up[x][i - 1] != -1; i++) {
			up[x][i] = up[up[x][i - 1]][i - 1];
		}
		for (int i : g.adj[x]) {
			i = g.edges[i].u ^ g.edges[i].v ^ x;
			if (i != p) {
				lvl[i] = lvl[x] + 1;
				dfs(i, x, g);
			}
		}
		tout[x] = timer++;
	}

	bool f(int u, int v) {
		return tin[u] <= tin[v] && tout[u] >= tout[v];
	}

	int lca(int u, int v) {
		if (f(u, v))
			return u;
		if (f(v, u))
			return v;
		for (int i = lg - 1; i >= 0; i--) {
			if (up[u][i] != -1 && !f(up[u][i], v)) {
				u = up[u][i];
			}
		}
		return up[u][0];
	}

	int dist(int u, int v) {
		return lvl[u] + lvl[v] - 2 * lvl[lca(u, v)];
	}
};