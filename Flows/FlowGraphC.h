#include <vector>

template <typename T, typename C = T>
struct flow_graph {
	struct edge {
		int u, v;
		T c, w; C f;
		edge(int u, int v, T c, C w, T f) : u(u), v(v), c(c), w(w), f(f) {}
	};

	int n, m;
	std::vector<edge> edges;
	std::vector<std::vector<int>> adj;

	flow_graph(): n(), m() {}
	flow_graph(int n, int m = 0): n(n), m(), adj(n) {
		edges.reserve(2 * m);
	}

	void add_edge(int u, int v, T c, C w) {
		edges.emplace_back(u, v, c, w, 0);
		adj[u].push_back(m++);
		edges.emplace_back(v, u, 0, -w, 0);
		adj[v].push_back(m++);
	}
};