#include "FlowGraphC.h"
#include <limits>
#include <queue>

template <typename T, typename C>
std::pair<T, C> min_cost_flow(flow_graph<T, C> &g, int s, int t, T target_flow = -1) {
	if (target_flow < 0) target_flow = std::numeric_limits<T>::max();
	static constexpr C inf_cost = std::numeric_limits<C>::max();

	std::vector<C> d(g.n);
	std::vector<int> p(g.n);
	std::vector<bool> in_queue(g.n);

	auto spfa = [&](void) -> void {
		std::fill(d.begin(), d.end(), inf_cost);
		std::fill(in_queue.begin(), in_queue.end(), false);

		std::queue<int> q;
		q.push(s);
		d[s] = 0, in_queue[s] = true;

		while (!q.empty()) {
			int x = q.front();
			q.pop(), in_queue[x] = false;

			for (int i : g.adj[x]) {
				const auto &e = g.edges[i];
				if (e.c - e.f > 0 && d[e.u] + e.w < d[e.v]) {
					d[e.v] = d[e.u] + e.w;
					p[e.v] = i;
					if (!in_queue[e.v]) {
						in_queue[e.v] = true;
						q.push(e.v);
					}
				}
			}
		}
	};

	T flow = 0;
	C cost = 0;

	while (flow < target_flow && (spfa(), d[t] != inf_cost)) {
		T path_flow = target_flow - flow;
		for (int x = t; x != s; x = g.edges[p[x]].u) {
			path_flow = std::min(path_flow, g.edges[p[x]].c - g.edges[p[x]].f);
		}

		cost += path_flow * d[t];
		for (int x = t; x != s; x = g.edges[p[x]].u) {
			int i = p[x];
			g.edges[i].f += path_flow;
			g.edges[i ^ 1].f -= path_flow;
		}
		flow += path_flow;
	}

	return std::make_pair(flow, cost);
}