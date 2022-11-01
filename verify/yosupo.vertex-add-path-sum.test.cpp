#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"

#include <bits/stdc++.h>
using namespace std;

#include "newgraphs/undirected-graph.hpp"
#include "trees/heavy-light-decomposition.hpp"
#include "trees/dynamic-path-query/commutative.hpp"
#include "data-structures/segment-tree/basic.hpp"
#include "algebra/groupoid/common.hpp"

struct point_add_range_sum : data_structures::segment_tree<algebra::sum_monoid<long long>> {
point_add_range_sum(auto&&...args) : data_structures::segment_tree<algebra::sum_monoid<long long>>(args...) {}

void point_add(int p, int x) {
    visit_point<true>(p, [&x](long long &y) {
        y += x;
    });
}
};

using path_query = trees::dynamic_path_query<graphs::undirected_graph<>, point_add_range_sum>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (auto &x : a) cin >> x;

    graphs::undirected_graph<> g(n, n - 1);
    for (int i = 0, u, v; i + 1 < n; i++) {
        cin >> u >> v;
        g.add_edge(u, v);
    }

    trees::heavy_light_decomposition<decltype(g)> hld(g);
    path_query query(hld, a);

    while (q--) {
        int type;
        cin >> type;

        if (type == 0) {
            int p, x;
            cin >> p >> x;
            query.vertex_update(&point_add_range_sum::point_add, p, x);
        } else {
            int u, v;
            cin >> u >> v;
            cout << query.path_query(u, v) << '\n';
        }
    }
    return 0;
}
