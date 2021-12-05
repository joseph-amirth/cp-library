#pragma once

#include <vector>

struct bipartite_graph {
    struct edge {
        int u, v;

        edge(int u, int v) : u(u), v(v) {}
    };

    int l, r, m;
    std::vector<edge> edges;
    std::vector<std::vector<int>> adj;

    std::vector<int> pl, pr;
    std::vector<bool> visited;

    bipartite_graph() : l(), r(), m() {}

    bipartite_graph(int l, int r, int m = 0) : l(l), r(r), m(), adj(l), pl(l, -1), pr(r, -1), visited(l) {
        edges.reserve(m);
    }

    int add_edge(int u, int v) {
        adj[u].push_back(v);
        edges.emplace_back(u, v);
        return m++;
    }

    bool dfs(int u) {
        visited[u] = true;
        for (int v: adj[u]) {
            if (pr[v] == -1) {
                pl[u] = v;
                pr[v] = u;
                return true;
            }
        }
        for (int v: adj[u]) {
            if (!visited[pr[v]] && dfs(pr[v])) {
                pl[u] = v;
                pr[v] = u;
                return true;
            }
        }
        return false;
    }

    int matching() {
        int ans = 0;
        while (true) {
            std::fill(visited.begin(), visited.end(), false);
            int add = 0;
            for (int u = 0; u < l; u++) {
                if (pl[u] == -1) {
                    add += dfs(u);
                }
            }
            if (!add) {
                break;
            }
            ans += add;
        }
        return ans;
    }
};