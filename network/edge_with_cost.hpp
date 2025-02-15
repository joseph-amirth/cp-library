#pragma once

#include "edge.hpp"
#include "type_traits.hpp"
#include <type_traits>

namespace network {

template <typename Flow = int, typename Cost = int>
struct edge_with_cost : edge<Flow> {
    using flow_type = Flow;
    using cost_type = Cost;

    cost_type cost;

    edge_with_cost() : edge<flow_type>() {}

    edge_with_cost(int u, int v, flow_type cap, cost_type cost) : edge<flow_type>(u, v, cap), cost(cost) {}
};

template <typename Flow, typename Cost>
struct is_edge_with_cost<edge_with_cost<Flow, Cost>> : std::true_type {};

} // namespace network
