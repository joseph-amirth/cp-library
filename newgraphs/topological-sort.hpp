#pragma once

#include <vector>

namespace graphs {

template <typename DirectedGraph>
std::vector<int> topological_sort(const DirectedGraph &g) {
    std::vector<int> deg(g.n);
    for (const auto &e : g.edges) {
        deg[e.v] += 1;
    }

    std::vector<int> q;
    q.reserve(g.n);

    for (int u = 0; u < g.n; u++) {
        if (deg[u] == 0) {
            q.push_back(u);
        }
    }

    for (int qi = 0; qi < (int) q.size(); qi++) {
        int u = q[qi];
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (--deg[v] == 0) {
                q.push_back(v);
            }
        }
    }

    if (q.size() != g.n) {
        return {};
    } else {
        return q;
    }
}

}
