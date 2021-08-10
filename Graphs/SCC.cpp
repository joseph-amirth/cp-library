#include "Graph.h"
#include <algorithm>

std::vector<std::vector<int>> find_sccs(const digraph &g) {
	int timer = 0;
	std::vector<int> tin(g.n, -1), low(g.n);

	std::vector<int> all, path;
	all.reserve(g.n), path.reserve(g.n);

	std::vector<std::vector<int>> sccs;
	auto dfs = [&](int u, const auto &self) -> void {
		low[u] = tin[u] = timer++;
		all.push_back(u), path.push_back(u);
		for (int i : g.adj[u]) {
			int v = g.edges[i].v;
			if (tin[v] == -1) {
				self(v, self);
			}
			low[u] = std::min(low[u], low[v]);
		}
		if (low[u] == tin[u]) {
			int sz = std::find(all.rbegin(), all.rend(), u) - all.rbegin();
			sccs.emplace_back(sz + 1);
			for (int i = 0; i < sz; i++) {
				low[all.back()] = g.n;
				sccs.back()[i] = all.back();
				all.pop_back();
			}
			all.pop_back();
			sccs.back().back() = u, low[u] = g.n;
		}
		path.pop_back();
	};

	for (int u = 0; u < g.n; u++) {
		if (tin[u] == -1) {
			dfs(u, dfs);
		}
	}
	std::reverse(sccs.begin(), sccs.end());
	return sccs;
}