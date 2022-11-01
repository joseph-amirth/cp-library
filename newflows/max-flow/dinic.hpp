#pragma once

#include "../network.hpp"
#include "../primitives/augment-path.hpp"

namespace networks {

template <typename Edge>
typename Edge::flow_type dinic_max_flow(network<Edge> &g, int s, int t, typename Edge::flow_type target_flow = -1) {
    using edge_type = Edge;
    using flow_type = typename edge_type::flow_type;

    if (target_flow < 0) target_flow = std::numeric_limits<flow_type>::max();

    std::vector<int> lvl(g.n), q;
    q.reserve(g.n);

    auto bfs = [&](void) -> void {
        std::fill(lvl.begin(), lvl.end(), -1);
        q.clear();

        q.push_back(s);
        lvl[s] = 0;

        for (int qi = 0; qi < (int) q.size(); qi++) {
            int u = q[qi];
            for (int i : g.adj[u]) {
                const auto &e = g.edges[i];
                if (lvl[e.v] == -1 && e.cap - e.flow > 0) {
                    lvl[e.v] = 1 + lvl[u];
                    q.push_back(e.v);
                }
            }
        }
    };

    std::vector<int> p(g.n), ptr(g.n);

    auto dfs = [&](int u, auto &&self) -> bool {
        if (u == t) return true;
        for (; ptr[u] < (int) g.adj[u].size(); ptr[u]++) {
            const auto &e = g.edges[g.adj[u][ptr[u]]];
            if (lvl[u] + 1 == lvl[e.v] && e.cap - e.flow > 0) {
                p[e.v] = g.adj[u][ptr[u]];
                if (self(e.v, self)) {
                    return true;
                }
            }
        }
        return false;
    };

    typename edge_type::flow_type flow = 0;
    while (flow < target_flow && (bfs(), lvl[t] != -1)) {
        while (flow < target_flow && dfs(s, dfs)) {
            flow += augment_path(g, s, t, p, target_flow - flow);
        }
        std::fill(ptr.begin(), ptr.end(), 0);
    }
    return flow;
}

}
