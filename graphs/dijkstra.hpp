#pragma once

#include "graph-weighted.hpp"
#include <queue>
#include <limits>

template<typename weight_t, typename distance_t = weight_t>
auto dijkstra(const graph<weight_t> &g, int s = 0) {
    std::vector<distance_t> dist(g.n, std::numeric_limits<distance_t>::max());
    std::vector<int> parent(g.n, -1);

    std::priority_queue<std::pair<distance_t, int>, std::vector<std::pair<distance_t, int>>, std::greater<>> q;
    dist[s] = 0, q.emplace(0, s);

    dist[s] = 0;
    while (!q.empty()) {
        auto[d, u] = q.top();
        q.pop();

        if (d != dist[u]) {
            continue;
        }

        for (int i: g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (dist[u] + g.edges[i].w < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + g.edges[i].w;
                q.emplace(dist[v], v);
            }
        }
    }
    return std::make_pair(dist, parent);
}