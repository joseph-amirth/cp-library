#include "FlowGraph.h"
#include <functional>
#include <queue>

template <typename T>
T max_flow(flow_graph<T> &g, int s, int t) {
	std::vector<int> lvl(g.n);

	auto bfs = [&](void) -> void {
		std::fill(lvl.begin(), lvl.end(), -1);
		std::queue<int> q;
		q.push(s);
		lvl[s] = 0;
		while (!q.empty()) {
			int x = q.front();
			q.pop();
			for (int i : g.adj[x]) {
				const auto &e = g.edges[i];
				if (lvl[e.v] == -1 && e.v - e.f > 0) {
					lvl[e.v] = 1 + lvl[x];
					q.push(e.v);
				}
			}
		}
	};

	std::vector<int> p(g.n), ptr(g.n);

	auto dfs = [&](int x, const auto &dfs) -> bool {
		if (x == t) return true;
		for (; ptr[x] < g.adj[x].size(); ptr[x]++) {
			const auto &e = g.edges[g.adj[x][ptr[x]]];
			if (lvl[x] + 1 == lvl[e.v] && e.v - e.f > 0) {
				p[e.v] = g.adj[x][ptr[x]];
				if (dfs(e.v, dfs)) {
					return true;
				}
			}
		}
		return false;
	};

	T max_flow = 0;
	while (bfs(), lvl[t] != -1) {
		while (dfs(s, dfs)) {
			T path_flow = std::numeric_limits<T>::max();
			for (int x = t; x != s; x = g.edges[p[x]].u) {
				path_flow = std::min(path_flow, g.edges[p[x]].v - g.edges[p[x]].f);
			}
			max_flow += path_flow;
			for (int x = t; x != s; x = g.edges[p[x]].u) {
				int i = p[x];
				g.edges[i].f += path_flow;
				g.edges[i ^ 1].f -= path_flow;
			}
		}
		std::fill(ptr.begin(), ptr.end(), 0);
	}
	return max_flow;
}