#pragma once

#include <vector>

// TODO(Test this)

namespace graphs {

template <typename Graph>
Graph condensation(const Graph &g, const std::vector<std::vector<int>> &sccs) {
    std::vector<int> scc_id(g.n);
    for (int i = 0; i < (int) sccs.size(); i++) {
        for (int u : sccs[i]) {
            scc_id[u] = i;
        }
    }

    Graph dag(sccs.size(), g.m);
    for (const auto &scc : sccs) {
        for (int u : scc) {
            for (int i : g.adj[u]) {
                int v = g.edges[i].u ^ g.edges[i].v ^ u;
                if (scc_id[u] != scc_id[v]) {
                    dag.add_edge(scc_id[u], scc_id[v]);
                }
            }
        }
    }

    return dag;
}

}
