#pragma once

#include "../graph/edge.hpp"

namespace network {

template <typename Flow = int>
struct edge : graph::edge {
    using flow_type = Flow;

    flow_type cap, flow;

    edge() : graph::edge(), cap(-1) {}

    edge(int u, int v, flow_type cap) : graph::edge(u, v), cap(cap), flow(0) {}
};

} // namespace network
