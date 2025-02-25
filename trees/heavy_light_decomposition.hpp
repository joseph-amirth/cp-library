#pragma once

#include "../graphs/undirected_graph.hpp"
#include <concepts>

namespace trees {

namespace {
template <typename T>
concept Visitor = std::invocable<T, int, int>;
}

template <typename Graph = graphs::undirected_graph<>>
struct heavy_light_decomposition {
    const Graph &g;
    std::vector<int> parent, head, depth;
    std::vector<int> tree_pos;

    heavy_light_decomposition(const Graph &g) : g(g), parent(g.n, -1), head(g.n, -1), depth(g.n), tree_pos(g.n, -1) {
        std::vector<int> ptr(g.n, -1);
        auto dfs = [&](auto &&self, int u) -> int {
            int tree_sz = 1, mx_sz = 0;
            for (int i : g.adj[u]) {
                int v = g.edges[i].u ^ g.edges[i].v ^ u;
                if (v == parent[u]) {
                    continue;
                }
                depth[v] = 1 + depth[u], parent[v] = u;
                int subtree_sz = self(self, v);
                tree_sz += subtree_sz;
                if (subtree_sz > mx_sz) {
                    ptr[u] = v;
                    mx_sz = subtree_sz;
                }
            }
            return tree_sz;
        };
        dfs(dfs, 0);
        for (int u = 0, nxt = g.n - 1; u < g.n; u++) {
            if (parent[u] == -1 || ptr[parent[u]] != u) {
                for (int v = u; v != -1; v = ptr[v]) {
                    tree_pos[v] = nxt--;
                    head[v] = u;
                }
            }
        }
    }

    template <Visitor Function>
    void unordered_visit_path(int u, int v, Function &&f) const {
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

    template <Visitor FunctionUp, Visitor FunctionDown>
    void semiordered_visit_path(int u, int v, FunctionUp &&f_up, FunctionDown &&f_down) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] >= depth[head[v]]) {
                f_up(tree_pos[u], tree_pos[head[u]]);
                u = parent[head[u]];
            } else {
                f_down(tree_pos[v], tree_pos[head[v]]);
                v = parent[head[v]];
            }
        }
        if (depth[u] >= depth[v]) {
            f_up(tree_pos[u], tree_pos[v]);
        } else {
            f_down(tree_pos[v], tree_pos[u]);
        }
    }

    template <Visitor FunctionUp, Visitor FunctionDown>
    void ordered_visit_path(int u, int v, FunctionUp &&f_up, FunctionDown &&f_down) const {
        static std::vector<std::pair<int, int>> right_segments;
        while (head[u] != head[v]) {
            if (depth[head[u]] >= depth[head[v]]) {
                f_up(tree_pos[u], tree_pos[head[u]]);
                u = parent[head[u]];
            } else {
                right_segments.emplace_back(tree_pos[v], tree_pos[head[v]]);
                v = parent[head[v]];
            }
        }
        if (depth[u] >= depth[v]) {
            f_up(tree_pos[u], tree_pos[v]);
        } else {
            f_down(tree_pos[v], tree_pos[u]);
        }
        while (!right_segments.empty()) {
            auto [l, r] = right_segments.back();
            f_down(l, r);
            right_segments.pop_back();
        }
    }
};

} // namespace trees
