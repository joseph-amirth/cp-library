#pragma once

#include "../edge.hpp"
#include "../edge_with_cost.hpp"
#include "../network.hpp"
#include "../primitives/augment_path.hpp"
#include <limits>
#include <utility>

namespace networks {

template <typename Edge>
std::pair<flow_t<Edge>, cost_t<Edge>>
successive_shortest_paths_method(network<Edge> &g, int s, int t, flow_t<Edge> target_flow, auto &&f) {
    using flow_type = flow_t<Edge>;
    using cost_type = cost_t<Edge>;

    constexpr cost_type inf_cost = std::numeric_limits<cost_type>::max();

    std::vector<int> p(g.n);

    flow_type flow = 0;
    cost_type cost = 0, distance;

    while (flow < target_flow && (distance = f(p), distance < inf_cost)) {
        flow_type path_flow = augment_path(g, s, t, p, target_flow - flow);
        cost += path_flow * distance;
        flow += path_flow;
    }

    return std::make_pair(flow, cost);
}

} // namespace networks
