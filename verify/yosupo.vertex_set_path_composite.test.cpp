#define PROBLEM "https://judge.yosupo.jp/problem/vertex_set_path_composite"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/affine_function.hpp"
#include "algebra/mint/static_mint.hpp"
#include "graph/undirected_graph.hpp"
#include "range_query/segment_tree/basic.hpp"
#include "trees/dynamic_path_query/general.hpp"
#include "trees/heavy_light_decomposition.hpp"

using mint = algebra::static_mint<998244353>;
using monoid = algebra::monoids::affine_function_composition<mint>;
using segtree = range_query::segment_tree<monoid>;
using path_query = trees::dynamic_path_query<graph::undirected_graph<>, segtree>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<algebra::affine_function<mint>> f(n);
    for (int i = 0; i < n; i++) {
        cin >> f[i].a >> f[i].b;
    }

    graph::undirected_graph<> g(n, n - 1);
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g.add_edge(u, v);
    }

    trees::heavy_light_decomposition hld(g);
    path_query query(hld, f.begin(), f.end());

    while (q--) {
        int type;
        cin >> type;

        if (type == 0) {
            int p;
            algebra::affine_function<mint> v;
            cin >> p >> v.a >> v.b;
            query.vertex_update(&segtree::point_assign, p, v);
        } else {
            int u, v;
            mint x;
            cin >> u >> v >> x;
            cout << query.path_query(u, v)(x) << '\n';
        }
    }
    return 0;
}
