#pragma once

#include "../newgraphs/edge.hpp"

namespace networks {

template <typename Flow = int>
struct edge : graphs::edge {
    using flow_type = Flow;

    flow_type cap, flow;

    edge() : graphs::edge(), cap(-1) {}

    edge(int u, int v, flow_type cap) : graphs::edge(u, v), cap(cap), flow(0) {}
};

template <typename Flow = int, typename Cost = Flow>
struct edge_with_cost : edge<Flow> {
    using flow_type = Flow;
    using cost_type = Cost;

    cost_type cost;

    edge_with_cost() : edge<flow_type>() {}

    edge_with_cost(int u, int v, flow_type cap, cost_type cost) : edge<flow_type>(u, v, cap), cost(cost) {}
};

} // namespace networks
