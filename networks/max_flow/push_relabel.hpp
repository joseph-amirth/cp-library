#pragma once

#include "../edge.hpp"
#include "../network.hpp"
#include <limits>

namespace networks {

template <typename Edge>
flow_t<Edge> push_relabel_max_flow(network<Edge> &g, int s, int t, flow_t<Edge> target_flow = -1) {
    using flow_type = flow_t<Edge>;

    if (target_flow < 0) {
        target_flow = std::numeric_limits<flow_type>::max();
    }

    std::vector<int> height(g.n);
    std::vector<flow_type> excess(g.n);

    auto push = [&](int i) -> void {
        int u = g.edges[i].u, v = g.edges[i].v;
        flow_type delta = std::min(excess[u], g.edges[i].cap - g.edges[i].flow);
        g.edges[i].flow += delta;
        g.edges[i ^ 1].flow -= delta;
        excess[u] -= delta;
        excess[v] += delta;
    };

    auto relabel = [&](int u) -> void {
        int min_height = 2 * g.n;
        for (int i : g.adj[u]) {
            const auto &e = g.edges[i];
            if (e.cap - e.flow > 0) {
                min_height = std::min(min_height, height[e.v]);
            }
        }
        if (min_height < 2 * g.n) {
            height[u] = min_height + 1;
        }
    };

    std::vector<int> max_height;
    auto find_vertices = [&](void) -> void {
        max_height.clear();
        for (int i = 0; i < g.n; i++) {
            if (i != s && i != t && excess[i] > 0) {
                if (!max_height.empty() && height[i] > height[max_height[0]])
                    max_height.clear();
                if (max_height.empty() || height[i] == height[max_height[0]])
                    max_height.push_back(i);
            }
        }
    };

    height[s] = g.n;
    excess[s] = target_flow;
    for (int i : g.adj[s]) {
        push(i);
    }

    while (find_vertices(), !max_height.empty()) {
        for (int u : max_height) {
            bool pushed = false;
            for (int i : g.adj[u]) {
                const auto &e = g.edges[i];
                if (e.cap - e.flow > 0 && height[u] == height[e.v] + 1) {
                    push(i);
                    pushed = true;
                }
            }
            if (!pushed) {
                relabel(u);
                break;
            }
        }
    }

    flow_type flow = 0;
    for (int i : g.adj[s]) {
        flow += g.edges[i].flow;
    }
    return flow;
}

} // namespace networks
