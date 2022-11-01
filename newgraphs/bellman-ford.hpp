#pragma once

#include "graph.hpp"

#include <limits>

namespace graphs {

template <typename Edge>
std::pair<std::vector<typename Edge::weight_type>, std::vector<int>>
bellman_ford_shortest_paths(const graph<Edge> &g, int s = 0) {
    using edge_type = Edge;
    using weight_type = typename edge_type::weight_type;

    constexpr weight_type inf_distance = std::numeric_limits<weight_type>::max();

    std::vector<weight_type> distance(g.n, inf_distance);
    std::vector<int> parent(g.n);

    distance[s] = 0;
    for (int k = 1; k < g.n; k++) {
        for (int u = 0; u < g.n; u++) {
            if (distance[u] == inf_distance) {
                continue;
            }
            for (int i : g.adj[u]) {
                int v = g.edges[i].u ^ g.edges[i].v ^ u;
                if (distance[u] + g.edges[i].w < distance[v]) {
                    distance[v] = distance[u] + g.edges[i].w;
                    parent[v] = i;
                }
            }
        }
    }

    return std::make_pair(distance, parent);
}

}
