#pragma once

#include "graph.hpp"
#include <algorithm>

std::vector<int> euler_circuit(const digraph &g) {
    int start;
    for (start = 0; start < g.n; start++) {
        if (!g.adj[start].empty()) {
            break;
        }
    }
    if (start == g.n) {
        return {};
    }

    std::vector<int> in(g.n), out(g.n);
    for (auto&[u, v]: g.edges) {
        out[u]++;
        in[v]++;
    }

    std::vector<int> st, circuit;
    circuit.reserve(g.m + 1);
    st.push_back(start);

    while (!st.empty()) {
        int x = st.back();
        if (out[x]) {
            int i = g.adj[x][--out[x]];
            st.push_back(g.edges[i].v);
        } else {
            st.pop_back();
            circuit.push_back(x);
        }
    }
    reverse(circuit.begin(), circuit.end());
    return circuit;
}

std::vector<int> euler_circuit(const graph &g) {
    int start;
    for (start = 0; start < g.n; start++) {
        if (!g.adj[start].empty()) break;
    }

    std::vector<int> deg(g.n), ind(g.n);
    for (int i = 0; i < g.n; i++) {
        deg[i] = ind[i] = g.adj[i].size();
    }

    std::vector<int> st;
    st.push_back(start);

    std::vector<int> circuit;
    circuit.reserve(g.m + 1);

    std::vector<bool> mark(g.m);
    while (!st.empty()) {
        int x = st.back();
        if (!deg[x]) {
            circuit.push_back(x);
            st.pop_back();
        } else {
            while (mark[g.adj[x][--ind[x]]]);
            int i = g.adj[x][ind[x]];
            mark[i] = true;
            i = x ^ g.edges[i].u ^ g.edges[i].v;
            deg[x]--;
            deg[i]--;
            st.push_back(i);
        }
    }
    return circuit;
}