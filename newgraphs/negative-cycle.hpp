#pragma once

#include "edge.hpp"
#include "graph.hpp"
#include "shortest-paths/bellman-ford.hpp"

namespace graphs {

template <typename Edge>
std::vector<int>
vertices_in_negative_cycle(const graph<Edge> &g, const std::vector<typename Edge::weight_type> &distance,
                           const std::vector<int> &parent) {
    using edge_type = Edge;
    using weight_type = typename edge_type::weight_type;


}

template <typename Edge>
std::vector<int> vertices_in_negative_cycle(const graph<Edge> &g) {
    auto [distance, parent] = bellman_ford_shortest_paths(g);
    return vertices_in_negative_cycle(g, distance, parent);
}

template <typename Edge>
bool detect_negative_cycle(const graph<Edge> &g) {
    auto distance = bellman_ford_shortest_paths(g).first;
    return detect_negative_cycle(g, distance);
}

template <typename Edge>
std::vector<int> find_negative_cycle(const graph<Edge> &g, const std::vector<typename Edge::weight_type> &distance,
                                     const std::vector<int> &parent) {

}

template <typename Edge>
std::vector<int> find_negative_cycle(const graph<Edge> &g) {
    auto [distance, parent] = bellman_ford_shortest_paths(g);
    return find_negative_cycle(g, distance, parent);
}

}
