#pragma once

#include "graph-weighted.hpp"
#include <limits>
#include <queue>

template<typename T>
std::vector<T> bellman_ford(const digraph<T> &g, int s) {
    constexpr T inf = std::numeric_limits<T>::max();
    std::vector<T> d(g.n, inf);
    d[s] = 0;
    for (int i = 1; i < g.n; i++) {
        for (const auto &e: g.edges) {
            if (d[e.u] != inf) {
                d[e.v] = min(d[e.v], d[e.u] + e.w);
            }
        }
    }
    return d;
}

template<typename T>
bool negative_cycle(const digraph<T> &g, const std::vector<T> &d) {
    for (int u = 0; u < g.n; u++) {
        for (const auto &e: g.edges) {
            if (d[e.v] > d[e.u] + e.w) {
                return true;
            }
        }
    }
    return false;
}

template<typename T>
std::vector<T> spfa(const digraph<T> &g, int s) {
    std::vector<bool> in_queue(g.n);
    std::vector<T> d(g.n, std::numeric_limits<T>::max());

    std::queue<int> q;
    q.push(s);
    d[s] = 0, in_queue[s] = true;

    while (!q.empty()) {
        int x = q.front();
        q.pop(), in_queue[x] = false;

        for (int i: g.adj[x]) {
            const auto &e = g.edges[i];
            if (d[x] + e.w < d[e.v]) {
                d[e.v] = d[x] + e.w;
                if (!in_queue[e.v]) {
                    in_queue[e.v] = true;
                    q.push(e.v);
                }
            }
        }
    }
    return d;
}