#pragma once

#include "../edge.hpp"
#include "../graph.hpp"

#include <limits>

namespace graphs {

template <typename Graph>
auto bellman_ford_shortest_paths(const Graph &g, int s = 0) {
    using graph_type = Graph;
    using edge_type = edge_t<graph_type>;
    using weight_sum_type = weight_sum_t<edge_type>;

    constexpr weight_sum_type inf_distance = std::numeric_limits<weight_sum_type>::max();

    std::vector<weight_sum_type> distance(g.n, inf_distance);
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

template <typename Graph>
bool detect_negative_cycle(const Graph &g, const std::vector<weight_sum_t<edge_t<Graph>>> &distance) {
    using graph_type = Graph;
    using edge_type = edge_t<graph_type>;
    using weight_sum_type = weight_sum_t<edge_type>;

    constexpr weight_sum_type inf_distance = std::numeric_limits<weight_sum_type>::max();

    for (int u = 0; u < g.n; u++) {
        if (distance[u] == inf_distance) {
            continue;
        }
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (distance[u] + g.edges[i].w < distance[v]) {
                return true;
            }
        }
    }
    return false;
}

}
