#include <queue>
#include <functional>
#include "FlowGraph.h"

template <typename T>
T max_flow(flow_graph<T> &g, int s, int t) {
	std::vector<int> p(g.n);

	auto bfs = [&](void) {
		std::fill(p.begin(), p.end(), 0);
		std::queue<int> q;
		q.emplace(s);
		p[s] = -1;
		while (!q.empty()) {
			int u = q.front();
			q.pop();
			for (int i : g.adj[u]) {
				const auto &e = g.edges[i];
				if (!p[e.v] and e.v - e.f) {
					p[e.v] = i;
					if (e.v == t) {
						return true;
					}
					q.push(e.v);
				}
			}
		}
		return false;
	};

	T max_flow = 0;
	while (bfs()) {
		T path_flow = std::numeric_limits<T>::max();
		for (int x = t; x != s; x = g.edges[p[x]].u) {
			path_flow = std::min(path_flow, g.edges[p[x]].v - g.edges[p[x]].f);
		}
		for (int x = t; x != s; x = g.edges[p[x]].u) {
			int i = p[x];
			g.edges[i].f += path_flow;
			g.edges[i ^ 1].f -= path_flow;
		}
		max_flow += path_flow;
	}
	return max_flow;
}