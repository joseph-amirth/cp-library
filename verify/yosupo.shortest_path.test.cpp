#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"

#include <bits/stdc++.h>
using namespace std;

#include "newgraphs/edge.hpp"
#include "newgraphs/directed-graph.hpp"
#include "newgraphs/shortest-paths/dijkstra.hpp"
#include "newgraphs/primitives/get-vertices-on-path.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace graphs;

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    directed_graph<weighted_edge<int, long long>> g(n, m);
    for (int i = 0, a, b, c; i < m; i++) {
        cin >> a >> b >> c;
        g.add_edge(a, b, c);
    }

    auto [distance, parent] = dijkstra_shortest_paths(g, s);

    if (parent[t] == -1) {
        return cout << "-1\n", 0;
    }

    auto path = get_vertices_on_path(g, s, t, parent);

    cout << distance[t] << ' ' << (int) path.size() - 1 << '\n';
    for (int i = 1; i < (int) path.size(); i++) {
        cout << path[i - 1] << ' ' << path[i] << '\n';
    }
    return 0;
}
