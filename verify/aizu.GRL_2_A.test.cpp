#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/2/GRL_2_A"

#include <bits/stdc++.h>
using namespace std;

#include "graphs/mst/kruskal.hpp"
#include "graphs/undirected_graph.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace graphs;

    int n, m;
    cin >> n >> m;

    undirected_graph<weighted_edge<int>> g(n, m);
    for (int i = 0, u, v, w; i < m; i++) {
        cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }

    cout << kruskal_mst(g).first << '\n';
    return 0;
}
