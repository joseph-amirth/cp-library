#pragma once

#include "../network.hpp"

#include <limits>

namespace networks {

template <typename Edge>
std::pair<typename Edge::flow_type, typename Edge::cost_type>
cycle_cancelling_min_cost_flow(network<Edge> &g, int s, int t, typename Edge::flow_type target_flow = -1) {
    using flow_type = typename Edge::flow_type;
    using cost_type = typename Edge::cost_type;

    if (target_flow < 0) target_flow = std::numeric_limits<flow_type>::max();

    flow_type flow = max_flow(g, s, t, target_flow);


}

}
