#define PROBLEM "https://judge.yosupo.jp/problem/eulerian_trail_undirected"

#include <bits/stdc++.h>
using namespace std;

#include "graphs/undirected/eulerian_trail.hpp"
#include "graphs/undirected_graph.hpp"

using ll = long long;
using graphs::find_eulerian_trail;
using graphs::undirected_graph;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        undirected_graph g(n, m);
        for (int i = 0, u, v; i < m; i++) {
            cin >> u >> v;
            g.add_edge(u, v);
        }

        auto maybe_trail = find_eulerian_trail(g);
        if (!maybe_trail.has_value()) {
            cout << "No" << '\n';
            continue;
        }

        cout << "Yes" << '\n';

        auto &trail = maybe_trail.value();

        for (int u : trail.vertices()) {
            cout << u << ' ';
        }
        cout << '\n';

        for (int i : trail.edges) {
            cout << i << ' ';
        }
        cout << '\n';
    }
    return 0;
}
