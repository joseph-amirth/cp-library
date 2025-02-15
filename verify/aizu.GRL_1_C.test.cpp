#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/1/GRL_1_C"

#include <bits/stdc++.h>
using namespace std;

#include "graph/directed_graph.hpp"
#include "graph/edge.hpp"
#include "graph/shortest_path/floyd_warshall.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace graph;

    int n, m;
    cin >> n >> m;

    directed_graph<weighted_edge<int>> g(n, m);
    for (int i = 0, a, b, c; i < m; i++) {
        cin >> a >> b >> c;
        g.add_edge(a, b, c);
    }

    auto distance = floyd_warshall(g);

    if (detect_negative_cycle(g, distance)) {
        cout << "NEGATIVE CYCLE\n";
    } else {
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                if (distance[u][v] == INT_MAX) {
                    cout << "INF";
                } else {
                    cout << distance[u][v];
                }
                cout << " \n"[v + 1 == n];
            }
        }
    }
    return 0;
}
