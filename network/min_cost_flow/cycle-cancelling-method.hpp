#pragma once

#include "../network.hpp"

#include <limits>

namespace networks {

template <typename Edge>
typename Edge::cost_type cycle_cancelling_method(network<Edge> &g, int s, int t, auto &&f) {
    using flow_type = typename Edge::flow_type;
    using cost_type = typename Edge::cost_type;

    constexpr flow_type inf_flow = std::numeric_limits<flow_type>::max();

    assert(g.is_feasible_flow(s, t));

    cost_type cost = g.get_cost();

    std::vector<int> cycle;
    while (cycle = f(g), !cycle.empty()) {
        flow_type cycle_flow = inf_flow;
        cost_type cycle_cost = 0;

        for (int i : cycle) {
            cycle_flow = std::min(cycle_flow, g.edges[i].cap - g.edges[i].flow);
            cycle_cost += g.edges[i].cost;
        }

        for (int i : cycle) {
            g.edges[i].flow += cycle_flow;
            g.edges[i ^ 1].flow -= cycle_flow;
        }

        cost += cycle_flow * cycle_cost;
    }

    return cost;
}

}
