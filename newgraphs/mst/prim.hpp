#pragma once

#include <queue>

#include "../edge.hpp"
#include "../graph.hpp"

// TODO(Test this out)

namespace graphs {

template <typename Graph>
std::pair<weight_sum_t<edge_t<Graph>>, std::vector<int>> prim_mst(const Graph &g, int s = 0) {
    using edge_type = edge_t<Graph>;
    using weight_sum_type = weight_sum_t<edge_type>;

    struct compare_edge_ids {
        std::vector<edge_type> &edges;
        bool operator()(int i, int j) const {
            return edges[i].w > edges[j].w;
        }
    };

    std::priority_queue<int, std::vector<int>, compare_edge_ids> pq(compare_edge_ids{g.edges});

    weight_sum_type mst_weight = 0;
    std::vector<int> mst;

    std::vector<bool> in_mst(g.n);
    in_mst[s] = true;

    for (int i : g.adj[s]) {
        pq.push(i);
    }

    while (!pq.empty()) {
        int ei = pq.top();
        pq.pop();

        const auto &e = g.edges[ei];
        if (in_mst[e.u] && in_mst[e.v]) {
            continue;
        }

        mst.push_back(ei);
        mst_weight += e.w;

        int u = (in_mst[e.u] ? e.v : e.u);
        in_mst[u] = true;

        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (!in_mst[v]) {
                pq.push(i);
            }
        }
    }

    return std::make_pair(mst_weight, mst);
}

}