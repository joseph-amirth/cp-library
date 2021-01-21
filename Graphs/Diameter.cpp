#include "Graph.h"
#include <functional>
#include <queue>

std::vector<int> diameter(const graph &g) {
	std::vector<int> lvl(g.n);
	auto bfs = [&](int s) {
		std::fill(lvl.begin(), lvl.end(), -1);
		std::queue<int> q;
		q.push(s);
		lvl[s] = 0;
		int furthest = s;
		while (!q.empty()) {
			int x = q.front();
			q.pop();
			for (int i : g.adj[x]) {
				i = g.edges[i].u ^ g.edges[i].v ^ x;
				if (lvl[i] == -1) {
					lvl[i] = 1 + lvl[x];
					q.push(i);
					if (lvl[i] > lvl[furthest]) {
						furthest = i;
					}
				}
			}
		}
		return furthest;
	};

	int f = bfs(0);

	std::vector<int> d;
	for (int x = bfs(f); x != f;) {
		d.push_back(x);
		for (int i : g.adj[x]) {
			if (lvl[i] + 1 == lvl[x]) {
				x = i;
				break;
			}
		}
	}
	d.push_back(f);
	return d;
}