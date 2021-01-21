#include "GraphW.h"
#include "../Data Structures/UnionFind.h"
#include <algorithm>

template <typename T>
std::vector<int> mst(const graph<T> &g) {
	union_find uf(g.n);

	std::vector<int> ind(g.m);
	std::iota(ind.begin(), ind.end(), 0);
	std::sort(ind.begin(), ind.end(), [&](int i, int j) {
		return g.edges[i].w < g.edges[j].w;
	});

	std::vector<int> mst;
	for (int i : ind) {
		if (uf.merge(g.edges[i].u, g.edges[i].v)) {
			mst.push_back(i);
		}
	}
	return mst;
}