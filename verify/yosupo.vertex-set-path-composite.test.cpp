#define PROBLEM "https://judge.yosupo.jp/problem/vertex_set_path_composite"

#include <bits/stdc++.h>
using namespace std;

#include "mint/static-mint.hpp"
#include "data-structures/segment-tree/basic.hpp"
#include "algebra/groupoid/function-composition.hpp"
#include "algebra/domain/affine-function.hpp"
#include "newgraphs/undirected-graph.hpp"
#include "trees/heavy-light-decomposition.hpp"
#include "trees/dynamic-path-query/general.hpp"

using mint = static_mint<998244353>;
using monoid = algebra::function_composition_monoid<algebra::affine_function<mint>>;
using segtree = data_structures::segment_tree<monoid>;
using path_query = trees::dynamic_path_query<graphs::undirected_graph<>, segtree>;

template <>
struct algebra::is_commutative<monoid> : std::false_type {};

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
            int p; algebra::affine_function<mint> v;
            cin >> p >> v.a >> v.b;
            query.vertex_update(&segtree::point_assign, p, v);
        } else {
            int u, v; mint x;
            cin >> u >> v >> x;
            cout << query.path_query(u, v)(x) << '\n';
        }
    }
    return 0;
}
