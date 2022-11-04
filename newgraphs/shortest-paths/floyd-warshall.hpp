#pragma once

#include "../graph.hpp"
#include "../edge.hpp"
#include <limits>

namespace graphs {

template <typename Graph, bool compute_paths = false>
auto floyd_warshall_all_pairs_shortest_paths(const Graph &g) {
    using graph_type = Graph;
    using edge_type = edge_t<graph_type>;
    using weight_sum_type = weight_sum_t<edge_type>;

    constexpr weight_sum_type inf = std::numeric_limits<weight_sum_type>::max();

    std::vector<std::vector<weight_sum_type>> distance(g.n, std::vector<weight_sum_type>(g.n, inf));

    std::vector<std::vector<int>> parent;
    if constexpr (compute_paths) {
        parent.assign(g.n, std::vector<int>(g.n));
    }

    for (int u = 0; u < g.n; u++) {
        distance[u][u] = 0;
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (g.edges[i].w < distance[u][v]) {
                distance[u][v] = g.edges[i].w;
                if constexpr (compute_paths) {
                    parent[u][v] = i;
                }
            }
        }
    }

    for (int k = 0; k < g.n; k++) {
        for (int u = 0; u < g.n; u++) {
            for (int v = 0; v < g.n; v++) {
                if (distance[u][k] != inf && distance[k][v] != inf && distance[u][k] + distance[k][v] < distance[u][v]) {
                    distance[u][v] = distance[u][k] + distance[k][v];
                    if constexpr (compute_paths) {
                        parent[u][v] = parent[k][v];
                    }
                }
            }
        }
    }

    if constexpr (compute_paths) {
        return std::make_pair(distance, parent);
    } else {
        return distance;
    }
}

template <typename Graph>
bool detect_negative_cycle(const Graph &g, const std::vector<std::vector<int>> &distance) {
    for (int u = 0; u < g.n; u++) {
        if (distance[u][u] < 0) {
            return true;
        }
    }
    return false;
}

}
