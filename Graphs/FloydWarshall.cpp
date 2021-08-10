#include "GraphW.h"
#include <limits>

template <typename T>
std::vector<std::vector<int>> floyd_warshall(const graph<T> &g) {
	static const T inf = std::numeric_limits<T>::max();
	std::vector<std::vector<T>> d(g.n, vector<T>(g.n, inf));
	for (int i = 0; i < g.n; i++) {
		d[i][i] = 0;
	}
	for (auto&[u, v, w] : g.edges) {
		d[u][v] = w;
	}
	for (int k = 0; k < g.n; k++) {
		for (int i = 0; i < g.n; i++) {
			for (int j = 0; j < g.n; j++) {
				if (d[i][k] != inf && d[k][j] != inf) {
					d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
				}
			}
		}
	}
	return d;
}