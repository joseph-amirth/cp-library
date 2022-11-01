#pragma once

#include "graph.hpp"
#include "bellman-ford.hpp"

namespace graphs {

template <typename TEdge>
std::vector<int>
vertices_in_negative_cycle(const graph<TEdge> &g, const std::vector<typename TEdge::weight_type> &distance,
                           const std::vector<int> &parent) {
    using edge_type = TEdge;
    using weight_type = typename edge_type::weight_type;


}

template <typename TEdge>
std::vector<int> vertices_in_negative_cycle(const graph<TEdge> &g) {
    auto [distance, parent] = bellman_ford_shortest_paths(g);
    return vertices_in_negative_cycle(g, distance, parent);
}

template <typename TEdge>
bool detect_negative_cycle(const graph<TEdge> &g, const std::vector<typename TEdge::weight_type> &distance) {
    using edge_type = TEdge;
    using weight_type = typename edge_type::weight_type;

    constexpr weight_type inf_distance = std::numeric_limits<weight_type>::max();

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

template <typename TEdge>
bool detect_negative_cycle(const graph<TEdge> &g) {
    auto distance = bellman_ford_shortest_paths(g).first;
    return detect_negative_cycle(g, distance);
}

template <typename TEdge>
std::vector<int> find_negative_cycle(const graph<TEdge> &g, const std::vector<typename TEdge::weight_type> &distance,
                                     const std::vector<int> &parent) {

}

template <typename TEdge>
std::vector<int> find_negative_cycle(const graph<TEdge> &g) {
    auto [distance, parent] = bellman_ford_shortest_paths(g);
    return find_negative_cycle(g, distance, parent);
}

}
