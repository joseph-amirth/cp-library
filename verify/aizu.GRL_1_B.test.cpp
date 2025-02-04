#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/1/GRL_1_B"

#include <bits/stdc++.h>
using namespace std;

#include "newgraphs/edge.hpp"
#include "newgraphs/directed-graph.hpp"
#include "newgraphs/shortest-paths/bellman-ford.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace graphs;

    int n, m, s;
    cin >> n >> m >> s;

    directed_graph<weighted_edge<int>> g(n, m);
    for (int i = 0, a, b, c; i < m; i++) {
        cin >> a >> b >> c;
        g.add_edge(a, b, c);
    }

    auto distance = bellman_ford_shortest_paths(g, s).first;

    if (detect_negative_cycle(g, distance)) {
        cout << "NEGATIVE CYCLE\n";
    } else {
        for (int u = 0; u < n; u++) {
            if (distance[u] == INT_MAX) {
                cout << "INF\n";
            } else {
                cout << distance[u] << '\n';
            }
        }
    }
    return 0;
}
