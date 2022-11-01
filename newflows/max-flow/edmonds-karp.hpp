#pragma once

#include "../network.hpp"
#include "../primitives/augment-path.hpp"
#include "ford-fulkerson-method.hpp"

namespace networks {

template <typename Edge>
typename Edge::flow_type edmonds_karp_max_flow(network<Edge> &g, int s, int t, typename Edge::flow_type target_flow = -1) {
    using edge_type = Edge;
    using flow_type = typename edge_type::flow_type;

    if (target_flow < 0) target_flow = std::numeric_limits<flow_type>::max();

    std::vector<int> q;
    q.reserve(g.n);

    return ford_fulkerson_method(g, s, t, target_flow, [&](std::vector<int> &p) -> bool {
        std::fill(p.begin(), p.end(), -2);
        q.clear();

        q.push_back(s);
        p[s] = -1;

        for (int qi = 0; qi < (int) q.size(); qi++) {
            int u = q[qi];
            for (int i : g.adj[u]) {
                const auto &e = g.edges[i];
                if (p[e.v] == -2 && e.cap - e.flow > 0) {
                    p[e.v] = i;
                    if (e.v == t) {
                        return true;
                    }
                    q.push_back(e.v);
                }
            }
        }
        return false;
    });
}

}
