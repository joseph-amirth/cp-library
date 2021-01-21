#include "Graph.h"

std::vector<int> topological_sort(const digraph &g) {
	std::vector<int> q, deg(g.n);
	q.reserve(g.n);

	for (auto&[u, v] : g.edges) {
		deg[v]++;
	}

	std::vector<bool> visited(g.n);
	for (int i = 0; i < g.n; i++) {
		if (deg[i] == 0) {
			visited[i] = true;
			q.push_back(i);
		}
	}

	for (int i = 0; i < int(q.size()); i++) {
		int x = q[i];
		for (int j : g.adj[x]) {
			const auto &e = g.edges[j];
			deg[e.v]--;
			if (deg[e.v] == 0) {
				visited[e.v] = true;
				q.push_back(e.v);
			}
		}
	}

	if (q.size() != g.n)
		q = {-1};
	return q;
}