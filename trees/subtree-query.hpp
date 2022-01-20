#pragma once

#include "../graphs/graph.hpp"
#include <functional>

template<typename T, typename segtree_t>
struct subtree_query { //vertex queries
    int timer = 0;
    std::vector<int> euler, tin, tout;
    segtree_t st;

    template<typename U, typename F>
    subtree_query(const graph &g, const U &a, int root, T e, F &&f) : tin(g.n), tout(g.n) {
        euler.reserve(2 * g.n - 1);
        dfs(root, -1, g);
        std::vector<T> values(2 * g.n - 1, e);
        for (int i = 0; i < g.n; i++) {
            values[tin[i]] = T(a[i]);
        }
        st = segtree_t(values, 2 * g.n - 1, e, f);
    }

    void dfs(int u, int p, const graph &g) {
        tin[u] = timer++;
        euler.push_back(u);
        for (int i: g.adj[u]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ u;
            if (v != p) {
                dfs(v, u, g);
                euler.push_back(u);
            }
        }
        tout[u] = timer++;
    }

    T query(int x) {
        return st.query(tin[x], tout[x]);
    }

    template<typename U>
    void update(int x, const U &val) {
        st.update(tin[x], val);
    }

    template<typename U>
    void update_subtree(int x, const U &val) {
        st.update(tin[x], tout[x], val);
    }
};