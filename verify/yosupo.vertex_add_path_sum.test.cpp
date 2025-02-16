#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/groups/sum.hpp"
#include "graph/undirected_graph.hpp"
#include "range_query/segment_tree/basic.hpp"
#include "trees/dynamic_path_query/commutative.hpp"
#include "trees/heavy_light_decomposition.hpp"

struct point_add_range_sum : range_query::segment_tree<algebra::groups::sum<long long>> {
    template <typename... Args>
    point_add_range_sum(Args &&...args) : range_query::segment_tree<algebra::groups::sum<long long>>(std::forward<Args>(args)...) {}

    void point_add(int p, int x) {
        visit_point<true>(p, [&x](long long &y) {
            y += x;
        });
    }
};

using path_query = trees::dynamic_path_query<graph::undirected_graph<>, point_add_range_sum>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (auto &x : a)
        cin >> x;

    graph::undirected_graph<> g(n, n - 1);
    for (int i = 0, u, v; i + 1 < n; i++) {
        cin >> u >> v;
        g.add_edge(u, v);
    }

    trees::heavy_light_decomposition<decltype(g)> hld(g);
    path_query query(hld, a.begin(), a.end());

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
