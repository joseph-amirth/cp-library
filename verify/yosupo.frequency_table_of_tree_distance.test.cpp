#define PROBLEM "https://judge.yosupo.jp/problem/frequency_table_of_tree_distance"

#include <bits/stdc++.h>
using namespace std;

#include "graphs/undirected_graph.hpp"
#include "polynomials/fft.hpp"
#include "trees/centroid_decomposition.hpp"

using ll = long long;
using graph = graphs::undirected_graph<>;
using polynomials::fft::convolution;
using trees::centroid_decomposition;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    graph g(n, n - 1);
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g.add_edge(u, v);
    }

    vector<ll> answer(n);
    centroid_decomposition(g, [&](int c, vector<bool> &visited) {
        vector<ll> depths;
        auto dfs = [&](auto &&self, int u, int p, int d) -> void {
            answer[d] += 1;
            while ((int)depths.size() <= d) {
                depths.emplace_back();
            }
            depths[d]++;
            for (int i : g.adj[u]) {
                int v = g.edges[i].u ^ g.edges[i].v ^ u;
                if (v != p && !visited[v]) {
                    self(self, v, u, d + 1);
                }
            }
        };

        vector<ll> tot_depths, sub;
        for (int i : g.adj[c]) {
            int v = g.edges[i].u ^ g.edges[i].v ^ c;
            if (!visited[v]) {
                depths.clear();
                dfs(dfs, v, c, 1);
                if (tot_depths.size() < depths.size()) {
                    tot_depths.resize(depths.size());
                }
                for (int i = 0; i < (int)depths.size(); i++) {
                    tot_depths[i] += depths[i];
                }
                depths = convolution(depths, depths);
                if (sub.size() < depths.size()) {
                    sub.resize(depths.size());
                }
                for (int i = 0; i < (int)depths.size(); i++) {
                    sub[i] += depths[i];
                }
            }
        }
        tot_depths = convolution(tot_depths, tot_depths);
        int sz = min(n, (int)tot_depths.size());
        for (int i = 0; i < sz; i++) {
            answer[i] += (tot_depths[i] - (i < (int)sub.size() ? sub[i] : 0)) / 2;
        }
    });

    for (int i = 1; i < n; i++) {
        cout << answer[i] << ' ';
    }
    cout << '\n';
    return 0;
}
