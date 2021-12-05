#pragma once

#include "heavy-light-decomposition.hpp"
#include <algorithm>

template<typename T, typename segtree_t>
struct commutative_vertex_path_query {
    const heavy_light &hld;
    segtree_t st;

    template<typename U, typename F>
    commutative_vertex_path_query(const heavy_light &hld, std::vector<U> &a, T &&e, F &&f) : hld(hld) {
        int n = hld.g.n;
        std::vector<T> values(n);
        for (int u = 0; u < n; u++) {
            values[hld.tree_pos[u]] = T(a[u]);
        }
        st = segtree_t(values, n, e, f);
    }

    T query(int u, int v) {
        T ans = st.e;
        hld.unordered_visit_path(u, v, [&](int l, int r) {
            ans = st.f(ans, st.query(l, r));
        });
        return ans;
    }

    template<typename U>
    void update(int u, const U &val) {
        st.update(hld.tree_pos[u], val);
    }

    template<typename U>
    void update_path(int u, int v, const U &val) {
        hld.unordered_visit_path(u, v, [&](int l, int r) {
            st.update(l, r, val);
        });
    }
};

template<typename T, typename segtree_t>
struct noncommutative_vertex_path_query {
    const heavy_light &hld;
    segtree_t st, reverse_st;

    template<typename U, typename F>
    noncommutative_vertex_path_query(const heavy_light &hld, std::vector<U> &a, T &&e, F &&f) : hld(hld) {
        int n = (int) hld.parent.size();
        std::vector<T> values(n);
        for (int u = 0; u < n; u++) {
            values[hld.tree_pos[u]] = T(a[u]);
        }
        st = segtree_t(values, n, e, f);
        std::reverse(values.begin(), values.end());
        reverse_st = segtree_t(values, n, e, f);
    }

    T query(int u, int v) {
        T ans = st.e;
        hld.ordered_visit_path(u, v, [&](int l, int r) {
            ans = st.f(ans, st.query(l, r));
        }, [&](int l, int r) {
            ans = st.f(ans, reverse_st.query(hld.g.n - 1 - r, hld.g.n - 1 - l));
        });
        return ans;
    }

    template<typename U>
    void update(int u, const U &val) {
        st.update(hld.tree_pos[u], val);
        reverse_st.update(hld.g.n - 1 - hld.tree_pos[u], val);
    }

    template<typename U>
    void update_path(int u, int v, const U &val) {
        hld.unordered_visit_path(u, v, [&](int l, int r) {
            st.update(l, r, val);
        });
    }
};