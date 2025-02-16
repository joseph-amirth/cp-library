#pragma once

#include "../graphs/edge.hpp"

namespace networks {

template <typename Flow = int>
struct edge : graphs::edge {
    using flow_type = Flow;

    flow_type cap, flow;

    edge() : graphs::edge(), cap(-1) {}

    edge(int u, int v, flow_type cap) : graphs::edge(u, v), cap(cap), flow(0) {}
};

} // namespace networks
