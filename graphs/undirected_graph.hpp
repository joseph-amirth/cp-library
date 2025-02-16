#pragma once

#include "concepts.hpp"
#include "edge.hpp"
#include "graph.hpp"

namespace graphs {

template <typename Edge = edge>
struct undirected_graph : public graph<Edge> {
    using edge_type = Edge;

    using graph<edge_type>::n;
    using graph<edge_type>::m;
    using graph<edge_type>::edges;
    using graph<edge_type>::adj;
    using graph<edge_type>::add_edge;

    undirected_graph(int n, int expected_m = 0) : graph<edge_type>(n, expected_m) {}

    virtual int add_edge(edge_type &&e) {
        edges.push_back(e);
        adj[e.u].push_back(m);
        adj[e.v].push_back(m);
        return m++;
    }
};

template <typename W, typename WSum>
struct is_weighted<undirected_graph<weighted_edge<W, WSum>>> : std::true_type {};

} // namespace graphs
