#define PROBLEM "https://judge.yosupo.jp/problem/vertex_set_path_composite"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/domain/affine-function.hpp"
#include "algebra/groupoid/function-composition.hpp"
#include "algebra/mint/static_mint.hpp"
#include "data-structures/segment-tree/basic.hpp"
#include "newgraphs/undirected-graph.hpp"
#include "trees/dynamic_path_query/general.hpp"
#include "trees/heavy-light-decomposition.hpp"

using mint = algebra::static_mint<998244353>;
using monoid = algebra::function_composition_monoid<algebra::affine_function<mint>>;
using segtree = data_structures::segment_tree<monoid>;
using path_query = trees::dynamic_path_query<graphs::undirected_graph<>, segtree>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<algebra::affine_function<mint>> f(n);
    for (int i = 0; i < n; i++) {
        cin >> f[i].a >> f[i].b;
    }

    graphs::undirected_graph<> g(n, n - 1);
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g.add_edge(u, v);
    }

    trees::heavy_light_decomposition hld(g);
    path_query query(hld, f);

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
