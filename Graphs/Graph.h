#include <vector>

struct graph {
	struct edge {
		int u, v;
		edge(int u, int v): u(u), v(v) {}
	};

	int n, m;
	std::vector<edge> edges;
	std::vector<std::vector<int>> adj;

	graph(): n(), m() {}
	graph(int n, int m = 0): n(n), m(), adj(n) {
		edges.reserve(m);
	}

	int add_edge(int u, int v) {
		adj[u].push_back(m);
		adj[v].push_back(m);
		edges.emplace_back(u, v);
		return m++;
	}
};

struct digraph {
	struct edge {
		int u, v;
		edge(int u, int v): u(u), v(v) {}
	};

	int n, m;
	std::vector<edge> edges;
	std::vector<std::vector<int>> adj;

	digraph(): n(), m() {}
	digraph(int n, int m = 0): n(n), m(), adj(n) {
		edges.reserve(m);
	}

	int add_edge(int u, int v) {
		edges.emplace_back(u, v);
		adj[u].push_back(m);
		return m++;
	}

	digraph transpose() const {
		digraph rev(n);
		for (auto&[u, v] : edges) {
			rev.add_edge(v, u);
		}
		return rev;
	}
};