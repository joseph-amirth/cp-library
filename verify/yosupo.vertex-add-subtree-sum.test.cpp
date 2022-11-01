#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_subtree_sum"

#include <bits/stdc++.h>
using namespace std;

#include "data-structures/segment-tree/basic.hpp"
#include "algebra/groupoid/common.hpp"
#include "trees/dynamic-subtree-query.hpp"

struct point_add_range_sum : data_structures::segment_tree<algebra::sum_monoid<long long>> {
    point_add_range_sum(auto&&...args) : data_structures::segment_tree<algebra::sum_monoid<long long>>(args...) {}

    void point_add(int p, int x) {
        visit_point<true>(p, [&x](auto &y) {
            y += x;
        });
    }
};

using subtree_query = trees::dynamic_subtree_query<graphs::undirected_graph<>, point_add_range_sum>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    std::vector<int> a(n);
    for (auto &x : a) cin >> x;

    graphs::undirected_graph g(n, n - 1);
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
            query.vertex_update(&point_add_range_sum::point_add, u, x);
        } else {
            int u;
            cin >> u;
            cout << query.subtree_query(u) << '\n';
        }
    }
    return 0;
}
