#pragma once

#include <algorithm>
#include "../newgraphs/undirected-graph.hpp"

template <typename Edge>
std::vector<int> centroid_decomposition(const graphs::undirected_graph<Edge> &g) {
    using edge_type = Edge;

    std::vector<int> c(g.n, -1), sz(g.n);

    auto dfs = [&](auto &&self, int u, int p) -> void {
        sz[u] = 1;
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (c[v] == -1 && v != p) {
                self(self, v, u);
                sz[u] += sz[v];
            }
        }
    };

    auto find_centroid = [&](auto &&self, int u, int p, int x) -> int {
        for (int i : g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (c[v] == -1 && v != p && 2 * sz[v] > sz[x]) {
                return self(self, v, u, x);
            }
        }
        return u;
    };

    auto decompose = [&](auto &&self, int x, int p) -> void {
        dfs(dfs, x, -1);
        int cen = find_centroid(find_centroid, x, -1, x);
        c[cen] = p;
        for (int i : g.adj[cen]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ cen;
            if (c[v] == -1) {
                self(self, v, cen);
            }
        }
    };

    decompose(decompose, 0, -2);

    *std::find(c.begin(), c.end(), -2) = -1;
    return c;
}
