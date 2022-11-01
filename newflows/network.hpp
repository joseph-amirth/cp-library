#pragma once

#include "edge.hpp"
#include "../newgraphs/directed-graph.hpp"

namespace networks {

template <typename Edge = edge<>>
struct network : public graphs::directed_graph<Edge> {
    using edge_type = Edge;
    using flow_type = typename edge_type::flow_type;
    using graph_type = graphs::directed_graph<edge_type>;

    using graph_type::n;
    using graph_type::m;
    using graph_type::edges;
    using graph_type::adj;

    network() : graph_type() {}

    network(int n, int expected_m = 0) : graph_type(n, expected_m) {}

    void add_edge_with_residual_edge(int u, int v, flow_type c, auto&&...args) {
        graph_type::add_edge(u, v, c, args...);
        graph_type::add_edge(v, u, 0, args...);
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

    template <typename T = Edge, typename U = typename T::cost_type>
    typename T::cost_type get_cost() {
        using cost_type = typename T::cost_type;
        cost_type cost = 0;
        for (int i = 0; i < m; i += 2) {
            cost += edges[i].flow * edges[i].cost;
        }
        return cost;
    }

    std::vector<flow_type> get_flow() {
        std::vector<flow_type> flow(m);
        for (int i = 0; i < m; i++) {
            flow[i] = edges[i].flow;
        }
        return flow;
    }

    void clear_flow() {
        for (const auto &e : edges) {
            e.flow = 0;
        }
    }
};

}
