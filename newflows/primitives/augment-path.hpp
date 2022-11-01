#pragma once

#include "../network.hpp"

#include <limits>

namespace networks {

template <typename Edge>
typename Edge::flow_type
augment_path(network<Edge> &g, int s, int t, const std::vector<int> &p, typename Edge::flow_type path_flow) {
    using edge_type = Edge;
    using flow_type = typename edge_type::flow_type;

    if (path_flow < 0) path_flow = std::numeric_limits<flow_type>::max();

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

}
