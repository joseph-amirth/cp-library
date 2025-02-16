#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include <bits/stdc++.h>
using namespace std;

#include "graphs/directed_graph.hpp"
#include "graphs/strong_connectivity/tarjan.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace graphs;

    int n, m;
    cin >> n >> m;

    directed_graph<> g(n, m);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        g.add_edge(u, v);
    }

    auto sccs = tarjan_sccs(g);

    cout << sccs.size() << '\n';
    for (const auto &scc : sccs) {
        cout << scc.size() << ' ';
        for (int u : scc) {
            cout << u << ' ';
        }
        cout << '\n';
    }
    return 0;
}
