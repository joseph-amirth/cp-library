#pragma once

#include "../graph/directed_graph.hpp"
#include "concepts.hpp"
#include "edge.hpp"
#include "type_traits.hpp"

namespace networks {

template <typename Edge = edge<>>
struct network : private graph::directed_graph<Edge> {
    using edge_type = Edge;
    using flow_type = flow_t<edge_type>;
    using graph_type = graph::directed_graph<edge_type>;

    using graph_type::adj;
    using graph_type::edges;
    using graph_type::m;
    using graph_type::n;

    network(int n, int expected_m = 0) : graph_type(n, expected_m) {}

    void add_edge_with_residual_edge(int u, int v, flow_type c)
        requires(!EdgeWithCost<edge_type>)
    {
        graph_type::add_edge(u, v, c);
        graph_type::add_edge(v, u, 0);
    }

    // TODO: Figure out a better architecture to avoid using auto here.
    void add_edge_with_residual_edge(int u, int v, flow_type c, auto d)
        requires EdgeWithCost<edge_type>
    {
        graph_type::add_edge(u, v, c, d);
        graph_type::add_edge(v, u, 0, -d);
    }

    bool is_feasible_flow(int s, int t) {
        bool is_feasible = true;
        for (int i = 0; i < m; i += 2) {
            is_feasible &= 0 <= edges[i].flow && edges[i].flow <= edges[i].cap;
            is_feasible &= edges[i].flow == -edges[i ^ 1].flow;
        }
        for (int u = 0; u < n; u++) {
            flow_type delta = 0;
            for (int i : adj[u]) {
                delta += edges[i].flow;
            }
            is_feasible &= delta == 0;
        }
        return is_feasible;
    }

    void clear_flow() {
        for (const auto &e : edges) {
            e.flow = 0;
        }
    }
};

} // namespace networks
