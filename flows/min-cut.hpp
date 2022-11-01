#pragma once

#include "flow-graph.hpp"

template <typename T>
std::vector<int> min_cut(flow_graph<T> &g, int s) {
    std::vector<bool> visited(g.n);

    std::vector<int> q;
    q.push_back(s);
    visited[s] = true;

    for (int qi = 0; qi < (int) q.size(); qi++) {
        int u = q[qi];
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (!visited[v] && g.edges[i].c - g.edges[i].f > 0) {
                q.push_back(v);
                visited[v] = true;
            }
        }
    }

    return q;
}
