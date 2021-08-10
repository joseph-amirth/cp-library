#include "Graph.h"

std::vector<int> topological_sort(const digraph &g) {
	std::vector<int> q, deg(g.n);
	q.reserve(g.n);

	for (const auto &e : g.edges) {
		deg[e.v] += 1;
	}

	for (int i = 0; i < g.n; i++) {
		if (deg[i] == 0) {
			q.push_back(i);
		}
	}

	for (int qi = 0; qi < (int) q.size(); qi++) {
		int u = q[qi];
		for (int i : g.adj[u]) {
			int v = g.edges[i].v;
			if (--deg[v] == 0) {
				q.push_back(v);
			}
		}
	}

	if (q.size() != g.n) {
		q = {-1};
	}
	return q;
}