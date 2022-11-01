#pragma once

#include "../network.hpp"
#include "../primitives/augment-path.hpp"
#include "successive-shortest-paths-method.hpp"

#include <limits>
#include <queue>

namespace networks {

template <typename Edge>
std::pair<typename Edge::flow_type, typename Edge::cost_type>
successive_shortest_paths_min_cost_flow(network<Edge> &g, int s, int t, typename Edge::flow_type target_flow = -1) {
    using flow_type = typename Edge::flow_type;
    using cost_type = typename Edge::cost_type;

    if (target_flow < 0) target_flow = std::numeric_limits<flow_type>::max();
    constexpr cost_type inf_cost = std::numeric_limits<cost_type>::max();

    std::vector<cost_type> d(g.n);
    std::vector<bool> in_queue(g.n);

    return successive_shortest_paths_method(g, s, t, target_flow, [&](std::vector<int> &p) -> cost_type {
        std::fill(d.begin(), d.end(), inf_cost);
        std::fill(in_queue.begin(), in_queue.end(), false);

        std::queue<int> q;
        q.push(s);
        d[s] = 0, in_queue[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop(), in_queue[u] = false;

            for (int i : g.adj[u]) {
                const auto &e = g.edges[i];
                if (e.cap - e.flow > 0 && d[u] + e.cost < d[e.v]) {
                    d[e.v] = d[u] + e.cost;
                    p[e.v] = i;
                    if (!in_queue[e.v]) {
                        in_queue[e.v] = true;
                        q.push(e.v);
                    }
                }
            }
        }

        return d[t];
    });
}

}
