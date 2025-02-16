#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_A"

#include <bits/stdc++.h>
using namespace std;

#include "networks/max_flow/edmonds_karp.hpp"
#include "networks/network.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace networks;

    int n, m;
    cin >> n >> m;

    network g(n, m);
    for (int i = 0, a, b, c; i < m; i++) {
        cin >> a >> b >> c;
        g.add_edge_with_residual_edge(a, b, c);
    }

    cout << edmonds_karp_max_flow(g, 0, n - 1) << '\n';
}
