#include <algorithm>
#include "../Graphs/Graph.h"

std::vector<int> centroid_decomposition(const graph &g) {
	std::vector<int> c(g.n), sz(g.n);

	auto dfs = [&](int u, int p, const auto &self) -> void {
		sz[u] = 1;
		for (int i : g.adj[u]) {
			int v = g.edges[i].u ^ g.edges[i].v ^ u;
			if (c[v] == -1 && v != p) {
				self(v, u, self);
				sz[u] += sz[v];
			}
		}
	};

	auto find_centroid = [&](int u, int p, int x, const auto &self) -> int {
		for (int i : g.adj[u]) {
			int v = g.edges[i].u ^ g.edges[i].v ^ u;
			if (c[v] == -1 && v != p && 2 * sz[v] > sz[x]) {
				return self(v, u, x, self);
			}
		}
		return u;
	};

	auto decompose = [&](int x, int p, const auto &self) -> void {
		dfs(x, -1, dfs);
		int cen = find_centroid(x, -1, x, find_centroid);
		c[cen] = p;
		for (int i : g.adj[cen]) {
			int v = g.edges[i].u ^ g.edges[i].v ^ cen;
			if (c[v] == -1) {
				self(v, cen, self);
			}
		}
	};

	decompose(0, -2, decompose);

	int root = std::find(c.begin(), c.end(), -2) - c.begin();
	return c[root] = -1, c;
}