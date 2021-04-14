#include <vector>

struct bigraph {
	struct edge {
		int u, v;
		edge(int u, int v) : u(u), v(v) {}
	};

	int l, r, m;
	std::vector<edge> edges;
	std::vector<std::vector<int>> adj;
	std::vector<int> pl, pr;
	std::vector<bool> visited;

	bigraph() : l(), r(), m() {}
	bigraph(int l, int r, int m = 0) : l(l), r(r), m(), adj(l), pl(l, -1), pr(r, -1), visited(l) {
		edges.reserve(m);
	}

	int add_edge(int u, int v) {
		adj[u].push_back(v);
		edges.emplace_back(u, v);
		return m++;
	}

	bool dfs(int x) {
		visited[x] = true;
		for (int i : adj[x]) {
			if (pr[i] == -1) {
				pl[x] = i;
				pr[i] = x;
				return true;
			}
		}
		for (int i : adj[x]) {
			if (!visited[pr[i]] && dfs(pr[i])) {
				pl[x] = i;
				pr[i] = x;
				return true;
			}
		}
		return false;
	}

	int matching() {
		int ans = 0;
		while (true) {
			std::fill(visited.begin(), visited.end(), false);
			int add = 0;
			for (int i = 0; i < l; i++)
				if (pl[i] == -1)
					add += dfs(i);
			if (!add) break;
			ans += add;
		}
		return ans;
	}
};