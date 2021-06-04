#include "Graph.h"
#include <algorithm>

std::vector<std::vector<int>> get_sccs(const digraph &g) {
	digraph gr = g.transpose();
	int timer = 0;
	std::vector<int> f(g.n), visited(g.n), l(g.n);

	auto dfs_rev = [&](int x, const auto &dfs_rev) -> void {
		visited[x] = true;
		for (int i : gr.adj[x]) {
			const auto &e = gr.edges[i];
			if (!visited[e.v]) {
				dfs_rev(e.v, dfs_rev);
			}
		}
		f[timer++] = x;
	};

	for (int i = 0; i < g.n; i++) {
		if (!visited[i]) {
			dfs_rev(i, dfs_rev);
		}
	}

	std::vector<int> cur_scc;
	auto dfs = [&](int x, const auto &dfs) -> void {
		visited[x] = true;
		cur_scc.push_back(x);
		for (int i : g.adj[x]) {
			const auto &e = g.edges[i];
			if (!visited[e.v]) {
				dfs(e.v, dfs);
			}
		}
	};

	std::fill(visited.begin(), visited.end(), false);
	std::vector<std::vector<int>> sccs;
	for (int i = g.n - 1; i >= 0; i--) {
		if (!visited[f[i]]) {
			cur_scc.clear();
			dfs(f[i], dfs);
			sccs.push_back(cur_scc);
		}
	}
	std::reverse(sccs.begin(), sccs.end());
	return sccs;
}