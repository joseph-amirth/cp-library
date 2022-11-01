#pragma once

#include "flow-graph.hpp"
#include <limits>

template <typename T>
T max_flow(flow_graph<T> &g, int s, int t) {
    std::vector<T> h(g.n), excess(g.n);

    auto push = [&](int i) -> void {
        int u = g.edges[i].u, v = g.edges[i].v;
        T d = std::min(excess[u], g.edges[i].cap - g.edges[i].flow);
        g.edges[i].f += d;
        g.edges[i ^ 1].f -= d;
        excess[u] -= d;
        excess[v] += d;
    };

    auto relabel = [&](int u) -> void {
        T d = 2 * g.n;
        for (int i : g.adj[u]) {
            if (g.edges[i].cap - g.edges[i].flow > 0) {
                d = std::min(d, h[g.edges[i].v]);
            }
        }
        if (d < 2 * g.n) {
            h[u] = d + 1;
        }
    };

    std::vector<int> max_height;

    auto find_vertices = [&](void) -> void {
        max_height.clear();
        for (int i = 0; i < g.n; i++) {
            if (i != s && i != t && excess[i] > 0) {
                if (!max_height.empty() && h[i] > h[max_height[0]])
                    max_height.clear();
                if (max_height.empty() || h[i] == h[max_height[0]])
                    max_height.push_back(i);
            }
        }
    };

    h[s] = g.n;
    excess[s] = std::numeric_limits<T>::max();
    for (int i : g.adj[s]) {
        push(i);
    }

    while (find_vertices(), !max_height.empty()) {
        for (int u : max_height) {
            bool pushed = false;
            for (int i : g.adj[u]) {
                if (g.edges[i].c - g.edges[i].f > 0 && h[u] == h[g.edges[i].v] + 1) {
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

    T max_flow = 0;
    for (int i : g.adj[s]) {
        max_flow += g.edges[i].f;
    }
    return max_flow;
}