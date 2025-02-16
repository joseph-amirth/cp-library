#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_B"

#include <bits/stdc++.h>
using namespace std;

#include "networks/edge.hpp"
#include "networks/min_cost_flow/successive_shortest_paths.hpp"
#include "networks/network.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace networks;

    int n, m, target_flow;
    cin >> n >> m >> target_flow;

    network<edge_with_cost<>> g(n, m);
    for (int i = 0, u, v, c, d; i < m; i++) {
        cin >> u >> v >> c >> d;
        g.add_edge_with_residual_edge(u, v, c, d);
    }

    auto [flow, cost] = successive_shortest_paths_min_cost_flow(g, 0, n - 1, target_flow);
    if (flow < target_flow) {
        cout << "-1" << '\n';
    } else {
        cout << cost << '\n';
    }
}
