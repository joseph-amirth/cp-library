#pragma once

#include "../network.hpp"

#include <limits>

namespace networks {

template <typename Edge>
std::pair<typename Edge::flow_type, typename Edge::cost_type>
successive_shortest_paths_method(network<Edge> &g, int s, int t, typename Edge::flow_type target_flow, auto &&f) {
    using flow_type = typename Edge::flow_type;
    using cost_type = typename Edge::cost_type;

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

}
