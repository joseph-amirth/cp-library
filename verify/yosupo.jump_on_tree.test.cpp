#define PROBLEM "https://judge.yosupo.jp/problem/jump_on_tree"

#include <bits/stdc++.h>
using namespace std;

#include "newgraphs/undirected-graph.hpp"
#include "tree/binary_lifting.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using graphs::undirected_graph;
    using tree::binary_lifting;

    int n, q;
    cin >> n >> q;

    undirected_graph g(n, n - 1);
    for (int i = 0, u, v; i + 1 < n; i++) {
        cin >> u >> v;
        g.add_edge(u, v);
    }

    binary_lifting bl(g);

    while (q--) {
        int s, t, i;
        cin >> s >> t >> i;

        cout << bl.kth_on_path(s, t, i) << '\n';
    }
    return 0;
}
