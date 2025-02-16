#pragma once

#include <vector>

namespace trees {

template <typename Graph>
struct compressed_euler_tour_tree {
    const Graph &g;
    std::vector<int> tin, tout;

    explicit compressed_euler_tour_tree(const Graph &g) : g(g), tin(g.n), tout(g.n) {
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

    std::vector<int> get_compressed_euler_tour() const {
        std::vector<int> euler_tour(g.n);
        for (int u = 0; u < g.n; u++) {
            euler_tour[tin[u]] = u;
        }
        return euler_tour;
    }
};

template <typename Graph>
struct euler_tour_tree {
    const Graph &g;
    std::vector<int> euler_tour, tin, tout;

    explicit euler_tour_tree(const Graph &g) : g(g), tin(g.n), tout(g.n) {
        int timer = 0;
        auto dfs = [&](auto &&self, int u, int p) -> void {
            tin[u] = timer++;
            euler_tour.push_back(u);
            for (int i : g.adj[u]) {
                if (i != p) {
                    int v = g.edges[i].u ^ g.edges[i].v ^ u;
                    self(self, v, i);
                    euler_tour.push_back(u);
                }
            }
            tout[u] = timer++;
        };
        dfs(dfs, 0, -1);
    }
};

} // namespace trees
