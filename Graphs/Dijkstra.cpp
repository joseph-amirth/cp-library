#include "GraphW.h"
#include <queue>
#include <limits>

template <typename T>
std::vector<T> dijkstra(const graph<T> &g, int s) {
	std::vector<bool> visited(g.n);
	std::vector<T> dist(g.n, std::numeric_limits<T>::max());

	std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<>> q;
	q.emplace(0, s);

	dist[s] = 0;
	while (!q.empty()) {
		int u = q.top().second;
		q.pop();

		if (visited[u])
			continue;

		visited[u] = true;
		for (int i : g.adj[u]) {
			const auto &e = g.edges[i];
			int v = u ^ e.u ^ e.v;
			if (dist[v] > dist[u] + e.w) {
				dist[v] = dist[u] + e.w;
				q.emplace(dist[v], v);
			}
		}
	}
	return dist;
}