#pragma once

#include "../graphs/graph.hpp"

struct heavy_light {
    std::vector<int> parent, head, depth, ptr;
    std::vector<int> tree_pos;

    heavy_light(const graph &g) : parent(g.n, -1), head(g.n, -1), depth(g.n), ptr(g.n, -1), tree_pos(g.n, -1) {
        auto dfs = [&](int u, const auto &self) -> int {
            int tree_sz = 1, mx_sz = 0;
            for (int i : g.adj[u]) {
                int v = g.edges[i].u ^ g.edges[i].v ^ u;
                if (v == parent[u]) {
                    continue;
                }
                depth[v] = 1 + depth[u], parent[v] = u;
                int subtree_sz = self(v, self);
                tree_sz += subtree_sz;
                if (subtree_sz > mx_sz) {
                    ptr[u] = v;
                    mx_sz = subtree_sz;
                }
            }
            return tree_sz;
        };
        dfs(0, dfs);
        for (int u = 0, nxt = g.n - 1; u < g.n; u++) {
            if (parent[u] == -1 || ptr[parent[u]] != u) {
                for (int v = u; v != -1; v = ptr[v]) {
                    tree_pos[v] = nxt--;
                    head[v] = u;
                }
            }
        }
    }

    template<typename F>
    void unordered_visit_path(int u, int v, F &&f) const {
        for (; head[u] != head[v]; u = parent[head[u]]) {
            if (depth[head[u]] < depth[head[v]]) {
                std::swap(u, v);
            }
            f(tree_pos[u], tree_pos[head[u]]);
        }
        if (depth[u] < depth[v]) {
            std::swap(u, v);
        }
        f(tree_pos[u], tree_pos[v]);
    }

    template<typename F, typename G>
    void ordered_visit_path(int u, int v, F &&f, G &&g) const {
        static std::vector<std::pair<int, int>> right_segments;
        while (head[u] != head[v]) {
            if (depth[head[u]] >= depth[head[v]]) {
                f(tree_pos[u], tree_pos[head[u]]);
                u = parent[head[u]];
            } else {
                right_segments.emplace_back(tree_pos[v], tree_pos[head[v]]);
                v = parent[head[v]];
            }
        }
        if (depth[u] >= depth[v]) {
            f(tree_pos[u], tree_pos[v]);
        } else {
            g(tree_pos[v], tree_pos[u]);
        }
        while (!right_segments.empty()) {
            auto[l, r] = right_segments.back();
            g(l, r);
            right_segments.pop_back();
        }
    }
};