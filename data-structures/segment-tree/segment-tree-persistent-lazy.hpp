#pragma once

#include <vector>
#include <functional>

template<typename T, typename L = T>
struct segment_tree {
    using F = std::function<T(const T &, const T &)>;

    struct node {
        T val;
        L lazy;
        int l, r;

        node(T val) : val(val), lazy() {}

        node(T val, int l, int r) : val(val), lazy(), l(l), r(r) {}

        node(T val, int lazy, int l, int r) : val(val), lazy(lazy), l(l), r(r) {}
    };

    std::vector<node> t;

    template<typename...args>
    int new_node(args...A) {
        int id = (int) t.size();
        t.emplace_back(A...);
        return id;
    }

    int n;
    std::vector<int> ver;
    T e;
    F f;

    segment_tree() : ver(), n(), e(), f() {}

    template<typename U>
    segment_tree(const std::vector<U> &v, int n, T e, F &&f): n(n), e(e), f(f) {
        ver.push_back(build(v, 0, n - 1));
    }

    template<typename U>
    int build(const std::vector<U> &v, int l, int r) {
        if (l == r)
            return new_node(T(v[l]));
        int mid = (l + r) >> 1;
        int il = build(v, l, mid);
        int ir = build(v, mid + 1, r);
        return new_node(f(t[il].val, t[ir].val), il, ir);
    }

    void push(int i, int l, int r) {
        //lazy
    }

    template<typename U>
    int update(int idx, const U &val, int k = -1) {
        if (k == -1) k = ver.back();
        ver.push_back(update_helper(idx, val, k, 0, n - 1));
        return ver.back();
    }

    template<typename U>
    int update_helper(int idx, const U &val, int i, int l, int r) {
        if (l == r)
            return new_node(T(val));
        push(i, l, r);
        int mid = (l + r) >> 1;
        if (idx <= mid) {
            int il = update_helper(idx, val, t[i].l, l, mid);
            return new_node(f(t[il].val, t[t[i].r].val), il, t[i].r);
        } else {
            int ir = update_helper(idx, val, t[i].r, mid + 1, r);
            return new_node(f(t[t[i].l].val, t[ir].val), t[i].l, ir);
        }
    }

    template<typename U>
    int update(int ql, int qr, const U &val, int k = -1) {
        if (k == -1) k = ver.back();
        ver.push_back(update_helper(ql, qr, val, k, 0, n - 1));
        return ver.back();
    }

    template<typename U>
    int update_helper(int ql, int qr, const U &val, int i, int l, int r) {
        if (r < ql || qr < l)
            return i;
        if (ql <= l && r <= qr) {
            //lazy
        }
        push(i, l, r);
        int mid = (l + r) >> 1;
        int il = update_helper(ql, qr, val, t[i].l, l, mid);
        int ir = update_helper(ql, qr, val, t[i].r, mid + 1, r);
        return new_node(f(t[il].val, t[ir].val), il, ir);
    }

    T query(int ql, int qr, int k = -1) {
        if (k == -1) k = ver.back();
        return query_helper(ql, qr, k, 0, n - 1);
    }

    T query_helper(int ql, int qr, int i, int l, int r) {
        if (r < ql || qr < l)
            return e;
        if (ql <= l && r <= qr)
            return t[i].val;
        push(i, l, r);
        int mid = (l + r) >> 1;
        return f(query_helper(ql, qr, t[i].l, l, mid), query_helper(ql, qr, t[i].r, mid + 1, r));
    }
};