#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_subtree_sum"

#include <bits/stdc++.h>
using namespace std;

#include "algebra/groups/sum.hpp"
#include "graph/undirected_graph.hpp"
#include "range_query/fenwick_tree.hpp"
#include "trees/dynamic_subtree_query.hpp"

namespace groups = algebra::groups;

using point_add_range_sum = range_query::fenwick_tree<groups::sum<long long>>;
using subtree_query = trees::dynamic_subtree_query<graph::undirected_graph<>, point_add_range_sum>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    std::vector<int> a(n);
    for (auto &x : a)
        cin >> x;

    graph::undirected_graph g(n, n - 1);
    for (int i = 1, p; i < n; i++) {
        cin >> p;
        g.add_edge(p, i);
    }

    trees::compressed_euler_tour_tree et(g);
    subtree_query query(et, a);

    while (q--) {
        int type;
        cin >> type;

        if (type == 0) {
            int u, x;
            cin >> u >> x;
            query.vertex_update(&point_add_range_sum::apply, u, x);
        } else {
            int u;
            cin >> u;
            cout << query.subtree_query(u) << '\n';
        }
    }
    return 0;
}
