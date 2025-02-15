#pragma once

#include "../edge.hpp"
#include "../undirected_graph.hpp"
#include <queue>

namespace graph {

template <typename Edge>
std::pair<weight_sum_t<Edge>, std::vector<int>> prim_mst(const undirected_graph<Edge> &g, int s = 0) {
    using weight_sum_type = weight_sum_t<Edge>;

    struct compare_edge_ids {
        const std::vector<Edge> &edges;
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

} // namespace graph
