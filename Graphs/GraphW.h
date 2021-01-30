#include <vector>

template <typename T>
struct graph {
	struct edge {
		int u, v; T w;
		edge(int u, int v, T w): u(u), v(v), w(w) {}
	};

	int n, m;
	std::vector<edge> edges;
	std::vector<std::vector<int>> adj;

	graph(): n(), m() {}
	graph(int n, int m = 0): n(n), m(), adj(n) {
		edges.reserve(m);
	}

	int add_edge(int u, int v, T w) {
		adj[u].emplace_back(m);
		adj[v].emplace_back(m);
		edges.emplace_back(u, v, w);
		return m++;
	}
};

template <typename T>
struct digraph {
	struct edge {
		int u, v; T w;
		edge(int u, int v, T w): u(u), v(v), w(w) {}
	};

	int n, m;
	std::vector<edge> edges;
	std::vector<std::vector<int>> adj;

	digraph(): n(), m() {}
	digraph(int n, int m = 0): n(n), m(), adj(n) {
		edges.reserve(m);
	}

	int add_edge(int u, int v, T w) {
		edges.emplace_back(u, v, w);
		adj[u].emplace_back(m);
		return m++;
	}

	digraph transpose() const {
		digraph rev(n);
		for (auto&[u, v, w] : edges) {
			rev.add_edge(v, u, w);
		}
		return rev;
	}
};