#pragma once

#include <numeric>

#include "../edge.hpp"
#include "../graph.hpp"
#include "../../data-structures/union-find/union-find.hpp"

namespace graphs {

template <typename Graph>
std::pair<weight_sum_t<edge_t<Graph>>, std::vector<int>> kruskal_mst(const Graph &g) {
    using edge_type = edge_t<Graph>;
    using weight_sum_type = weight_sum_t<edge_type>;

    std::vector<int> ids(g.m);
    std::iota(ids.begin(), ids.end(), 0);
    sort(ids.begin(), ids.end(), [&g](int i, int j) {
        return g.edges[i].w < g.edges[j].w;
    });

    union_find uf(g.n);

    weight_sum_type mst_weight = 0;

    std::vector<int> mst;
    for (int i : ids) {
        const auto &e = g.edges[i];
        if (uf.merge(e.u, e.v)) {
            mst.push_back(i);
            mst_weight += g.edges[i].w;
        }
    }

    return std::make_pair(mst_weight, mst);
}

}
