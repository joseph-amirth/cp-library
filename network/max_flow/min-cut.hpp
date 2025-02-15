#pragma once

#include "../network.hpp"

namespace networks {

template <typename Edge>
std::pair<std::vector<Edge>, std::vector<bool>> min_cut(network<Edge> &g, int s) {
    using edge_type = Edge;

    std::vector<bool> visited(g.n);

    std::vector<int> q;
    q.push_back(s);
    visited[s] = true;

    for (int qi = 0; qi < (int) q.size(); qi++) {
        int u = q[qi];
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (!visited[v] && g.edges[i].cap - g.edges[i].flow > 0) {
                q.push_back(v);
                visited[v] = true;
            }
        }
    }

    std::vector<edge_type> cut;
    for (const auto &e : g.edges) {
        if (visited[e.u] && !visited[e.v]) {
            cut.push_back(e);
        }
    }

    return std::make_pair(cut, visited);
}

}
