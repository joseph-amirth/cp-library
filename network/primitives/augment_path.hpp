#pragma once

#include "../edge.hpp"
#include "../network.hpp"
#include <limits>

namespace network {

template <typename Edge>
flow_t<Edge> augment_path(network<Edge> &g, int s, int t, const std::vector<int> &p, flow_t<Edge> path_flow) {
    using flow_type = flow_t<Edge>;
    if (path_flow < 0) {
        path_flow = std::numeric_limits<flow_type>::max();
    }
    for (int x = t; x != s; x = g.edges[p[x]].u) {
        path_flow = std::min(path_flow, g.edges[p[x]].cap - g.edges[p[x]].flow);
    }
    for (int x = t; x != s; x = g.edges[p[x]].u) {
        int i = p[x];
        g.edges[i].flow += path_flow;
        g.edges[i ^ 1].flow -= path_flow;
    }
    return path_flow;
}

} // namespace network
