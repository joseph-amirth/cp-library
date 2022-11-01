#pragma once

#include "../network.hpp"
#include "ford-fulkerson-method.hpp"

#include <limits>

namespace networks {

template <typename Edge>
typename Edge::flow_type ford_fulkerson_max_flow(network<Edge> &g, int s, int t, typename Edge::flow_type target_flow = -1) {
    using flow_type = typename Edge::flow_type;

    if (target_flow < 0) target_flow = std::numeric_limits<flow_type>::max();

    return ford_fulkerson_method(g, s, t, target_flow, [&](std::vector<int> &p) -> bool {
        std::fill(p.begin(), p.end(), -2);

        auto dfs = [&](int u, auto &&self) -> void {
            for (int i : g.adj[u]) {
                const auto &e = g.edges[i];
                if (p[e.v] == -2 && e.cap - e.flow > 0) {
                    p[e.v] = i;
                    self(e.v, self);
                }
            }
        };

        p[s] = -1;
        dfs(s, dfs);
        return p[t] != -2;
    });
}

}
