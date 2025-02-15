#define PROBLEM "https://judge.yosupo.jp/problem/minimum_spanning_tree"

#include <bits/stdc++.h>
using namespace std;

#include "graph/edge.hpp"
#include "graph/mst/kruskal.hpp"
#include "graph/undirected_graph.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using graph::kruskal_mst;
    using graph::undirected_graph;
    using graph::weighted_edge;

    int n, m;
    cin >> n >> m;

    undirected_graph<weighted_edge<int, long long>> g(n, m);
    for (int i = 0, u, v, w; i < m; i++) {
        cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }

    auto [cost, mst] = kruskal_mst(g);
    cout << cost << '\n';
    for (int i : mst)
        cout << i << ' ';
    cout << '\n';
    return 0;
}
