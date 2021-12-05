#pragma once

#include "graph.hpp"
#include <functional>
#include <queue>

std::vector<int> diameter(const graph &g) {
    std::vector<int> lvl(g.n);
    auto bfs = [&](int s) {
        std::fill(lvl.begin(), lvl.end(), -1);

        std::queue<int> q;
        q.push(s), lvl[s] = 0;

        int furthest = s;
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int i: g.adj[u]) {
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

    int f = bfs(0);

    std::vector<int> d;
    for (int u = bfs(f); u != f;) {
        d.push_back(u);
        for (int i: g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (lvl[v] + 1 == lvl[u]) {
                u = v;
                break;
            }
        }
    }
    d.push_back(f);
    return d;
}