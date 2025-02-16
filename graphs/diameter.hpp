#pragma once

#include <queue>

#include "edge.hpp"
#include "graph.hpp"
#include "primitives/get_vertices_on_path.hpp"

// TODO(Test this)

namespace graphs {

template <typename Graph>
std::vector<int> diameter(const Graph &g) {
    std::vector<int> lvl(g.n);
    auto bfs = [&](int s) -> int {
        std::fill(lvl.begin(), lvl.end(), -1);

        std::queue<int> q;
        lvl[s] = 0;
        q.push(s);

        int furthest = s;
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int i : g.adj[u]) {
                int v = g.edges[i].u ^ g.edges[i].v ^ u;
                if (lvl[v] == -1) {
                    lvl[v] = 1 + lvl[u];
                    q.push(v);
                    if (lvl[v] > lvl[furthest]) {
                        furthest = v;
                    }
                }
            }
        }

        return furthest;
    };

    int v = bfs(0);
    int u = bfs(v);

    return get_vertices_on_path(g, u, v, [&](int u) {
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (lvl[v] + 1 == lvl[u]) {
                return v;
            }
        }
        return -1;
    });
}

} // namespace graphs
