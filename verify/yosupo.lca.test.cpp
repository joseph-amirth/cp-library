#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include <bits/stdc++.h>
using namespace std;

#include "graph/undirected_graph.hpp"
#include "trees/euler_tour_tree.hpp"
#include "trees/lowest_common_ancestor.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using graph::undirected_graph;
    using trees::euler_tour_tree;
    using trees::lowest_common_ancestor;

    int n, q;
    cin >> n >> q;

    undirected_graph g(n, n - 1);
    for (int u = 1, p; u < n; u++) {
        cin >> p, g.add_edge(p, u);
    }

    euler_tour_tree ett(g);
    lowest_common_ancestor lca(ett);

    while (q--) {
        int u, v;
        cin >> u >> v;

        cout << lca.lca(u, v) << '\n';
    }
    return 0;
}
