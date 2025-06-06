#pragma once

#include "graphs/undirected_graph.hpp"

namespace trees {

template <typename Edge>
void centroid_decomposition(const graphs::undirected_graph<Edge> &g, auto &&f) {
    std::vector<bool> visited(g.n);
    std::vector<int> sz(g.n);

    auto find_szs = [&](auto &&self, int u, int p) -> void {
        sz[u] = 1;
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (!visited[v] && v != p) {
                self(self, v, u);
                sz[u] += sz[v];
            }
        }
    };

    auto find_centroid = [&](auto &&self, int x, int u, int p) -> int {
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (!visited[v] && v != p && 2 * sz[v] > sz[x]) {
                return self(self, x, v, u);
            }
        }
        return u;
    };

    auto decompose = [&](auto &&self, int x) -> void {
        find_szs(find_szs, x, -1);
        int c = find_centroid(find_centroid, x, x, -1);
        f(c, visited);
        visited[c] = true;
        for (int i : g.adj[c]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ c;
            if (!visited[v]) {
                self(self, v);
            }
        }
    };

    decompose(decompose, 0);
}

} // namespace trees
