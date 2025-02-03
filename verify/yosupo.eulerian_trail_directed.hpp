#define PROBLEM "https://judge.yosupo.jp/problem/eulerian_trail_directed"

#include <bits/stdc++.h>
using namespace std;

#include "newgraphs/directed-graph.hpp"
#include "newgraphs/eulerian_trail.hpp"

using graphs::directed_graph;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        graphs::directed_graph g(n, m);
        for (int i = 0, u, v; i < m; i++) {
            cin >> u >> v;
            g.add_edge(u, v);
        }

        auto maybe_eulerian_trail = eulerian_trail_checked(g);
    }
}
