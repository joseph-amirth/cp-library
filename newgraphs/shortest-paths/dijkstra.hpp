#pragma once

#include <queue>
#include <limits>

#include "../graph.hpp"
#include "../edge.hpp"

namespace graphs {

template <typename Graph>
auto dijkstra_shortest_paths(const Graph &g, int s = 0) {
    using graph_type = Graph;
    using edge_type = edge_t<graph_type>;
    using weight_sum_type = weight_sum_t<edge_type>;

    constexpr weight_sum_type inf = std::numeric_limits<weight_sum_type>::max();

    std::vector<weight_sum_type> distance(g.n, inf);
    std::vector<int> parent(g.n, -1);

    using pair_type = std::pair<weight_sum_type, int>;

    std::priority_queue<pair_type, std::vector<pair_type>, std::greater<>> pq;
    distance[s] = 0;
    pq.emplace(distance[s], s);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (distance[u] < d) {
            continue;
        }

        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (distance[u] + g.edges[i].w < distance[v]) {
                distance[v] = distance[u] + g.edges[i].w;
                parent[v] = i;
                pq.emplace(distance[v], v);
            }
        }
    }

    return std::make_pair(distance, parent);
}

}
