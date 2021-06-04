#include "SCC.cpp"

std::vector<bool> two_sat(int n, const std::vector<std::pair<int, int>> &clauses) {
	digraph g(2 * n);
	for (auto&[x, y] : clauses) {
		g.add_edge(x < n ? x + n : x - n, y);
		g.add_edge(y < n ? y + n : y - n, x);
	}

	auto sccs = get_sccs(g);

	std::vector<int> t(2 * n);
	for (int i = 0; i < sccs.size(); i++) {
		for (int x : sccs[i]) {
			t[x] = i;
		}
	}

	std::vector<bool> assignment(n);
	for (int i = 0; i < n; i++) {
		if (t[i] == t[i + n]) {
			return {};
		}
		assignment[i] = t[i] > t[i + n];
	}
	return assignment;
}