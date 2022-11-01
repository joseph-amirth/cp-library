#pragma once

#include <vector>

namespace trees {

template <typename Graph>
struct compressed_euler_tour_tree {
    using graph_type = Graph;

    const graph_type &g;
    std::vector<int> tin, tout;

    explicit compressed_euler_tour_tree(const graph_type &g) : g(g), tin(g.n), tout(g.n) {
        int timer = -1;
        auto dfs = [&](auto &&self, int u, int p) -> void {
            tin[u] = ++timer;
            for (int i : g.adj[u]) {
                if (i != p) {
                    int v = g.edges[i].u ^ g.edges[i].v ^ u;
                    self(self, v, i);
                }
            }
            tout[u] = timer;
        };
        dfs(dfs, 0, -1);
    }
};

template <typename Graph>
struct euler_tour_tree {
    using graph_type = Graph;

    const graph_type &g;
    std::vector<int> euler, tin, tout;

    explicit euler_tour_tree(const graph_type &g) : g(g), tin(g.n), tout(g.n) {
        int timer = 0;
        auto dfs = [&](auto &&self, int u, int p) -> void {
            tin[u] = timer++;
            euler.push_back(u);
            for (int i : g.adj[u]) {
                if (i != p) {
                    int v = g.edges[i].u ^ g.edges[i].v ^ u;
                    self(self, v, i);
                    euler.push_back(u);
                }
            }
            tout[u] = timer++;
        };
        dfs(dfs, 0, -1);
    }
};

}
