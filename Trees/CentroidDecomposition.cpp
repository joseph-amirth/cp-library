#include "../Graphs/Graph.h"

struct centroid_decomposition {
	const graph &g;
	std::vector<int> sz, c;

	void decompose(int x, int p) {
		dfs(x, -1);
		int cen = find_centroid(x, -1, x);
		c[cen] = p;
		for (int i : g.adj[cen]) {
			int v = g.edges[i].u ^ g.edges[i].v ^ cen;
			if (c[v] == -1) {
				decompose(v, cen);
			}
		}
	}

	void dfs(int u, int p) {
		sz[u] = 1;
		for (int i : g.adj[u]) {
			int v = g.edges[i].u ^ g.edges[i].v ^ u;
			if (c[v] == -1 && v != p) {
				dfs(v, u);
				sz[u] += sz[v];
			}
		}
	}

	int find_centroid(int u, int p, int x) {
		for (int i : g.adj[u]) {
			int v = g.edges[i].u ^ g.edges[i].v ^ u;
			if (c[v] == -1 && v != p && 2 * sz[v] > sz[x]) {
				return find_centroid(v, u, x);
			}
		}
		return u;
	}

	centroid_decomposition(const graph &g) : g(g), sz(g.n), c(g.n, -1) {
		decompose(0, -2);
	}

	template <typename F>
	void traverse_up(int x, const F &f) {
		for (; x != -2; x = c[x]) {
			f(x);
		}
	}
};
