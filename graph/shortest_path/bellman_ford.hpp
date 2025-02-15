#pragma once

#include "../edge.hpp"
#include "../graph.hpp"
#include <limits>
#include <utility>
#include <vector>

namespace graph {

template <typename Graph, bool compute_paths = false>
auto bellman_ford(const Graph &g, int s = 0) {
    using weight_sum_type = weight_sum_t<edge_t<Graph>>;

    constexpr weight_sum_type inf_distance = std::numeric_limits<weight_sum_type>::max();

    std::vector<weight_sum_type> distance(g.n, inf_distance);
    std::vector<int> parent;
    if constexpr (compute_paths) {
        parent.assign(g.n, -1);
    }

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
                    if constexpr (compute_paths) {
                        parent[v] = i;
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
bool detect_negative_cycle(const Graph &g, const std::vector<weight_sum_t<edge_t<Graph>>> &distance) {
    using weight_sum_type = weight_sum_t<edge_t<Graph>>;

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

} // namespace graph
