#pragma once

#include "../graph/undirected_graph.hpp"

namespace tree {

template <typename Edge>
struct binary_lifting {
    int lg;
    std::vector<int> tin, tout, lvl;
    std::vector<std::vector<int>> up;

    binary_lifting(const graph::undirected_graph<Edge> &g, int root = 0) : tin(g.n), tout(g.n), lvl(g.n) {
        lg = 32 - __builtin_clz(g.n);
        up.assign(g.n, std::vector<int>(lg, -1));

        int timer = 0;
        auto dfs = [&](int u, const auto &self) -> void {
            tin[u] = timer++;
            for (int i = 1; i < lg && up[u][i - 1] != -1; i++) {
                up[u][i] = up[up[u][i - 1]][i - 1];
            }
            for (int i : g.adj[u]) {
                int v = g.edges[i].u ^ g.edges[i].v ^ u;
                if (v != up[u][0]) {
                    lvl[v] = lvl[u] + 1;
                    up[v][0] = u;
                    self(v, self);
                }
            }
            tout[u] = timer++;
        };

        dfs(root, dfs);
    }

    bool is_ancestor(int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    int lca(int u, int v) {
        if (is_ancestor(u, v)) {
            return u;
        } else if (is_ancestor(v, u)) {
            return v;
        }
        for (int i = lg - 1; i >= 0; i--) {
            if (up[u][i] != -1 && !is_ancestor(up[u][i], v)) {
                u = up[u][i];
            }
        }
        return up[u][0];
    }

    int dist(int u, int v) {
        return lvl[u] + lvl[v] - 2 * lvl[lca(u, v)];
    }

    int kth_ancestor(int u, int k) {
        if (k < 0) {
            return -1;
        }
        for (int bit = 0; bit < lg; bit++) {
            if (k >> bit & 1) {
                u = up[u][bit];
            }
        }
        return u;
    }

    int kth_on_path(int u, int v, int k) {
        int l = lca(u, v);
        if (k <= lvl[u] - lvl[l]) {
            return kth_ancestor(u, k);
        }
        int dist = lvl[u] + lvl[v] - 2 * lvl[l];
        return kth_ancestor(v, dist - k);
    }
};

} // namespace tree
