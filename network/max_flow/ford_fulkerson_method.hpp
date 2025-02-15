#pragma once

#include "../edge.hpp"
#include "../network.hpp"
#include "../primitives/augment_path.hpp"

namespace network {

template <typename Edge>
flow_t<Edge> ford_fulkerson_method(network<Edge> &g, int s, int t, flow_t<Edge> target_flow, auto &&f) {
    using flow_type = flow_t<Edge>;

    std::vector<int> p(g.n);
    flow_type flow = 0;
    while (flow < target_flow && f(p)) {
        flow += augment_path(g, s, t, p, target_flow - flow);
    }
    return flow;
}

} // namespace network
