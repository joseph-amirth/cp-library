#pragma once

#include "graphs/undirected_graph.hpp"
#include "trail.hpp"

#include <algorithm>
#include <optional>

namespace graphs {

template <typename Edge>
std::optional<trail<Edge>> find_eulerian_trail(const undirected_graph<Edge> &g) {
    int odd_degs = std::count_if(g.adj.begin(), g.adj.end(), [](auto &v) { return v.size() % 2 != 0; });

    int start = g.n;
    if (odd_degs == 0) {
        start = std::find_if(g.adj.begin(), g.adj.end(), [](auto &v) { return !v.empty(); }) - g.adj.begin();
        if (start == g.n) {
            start = 0;
        }
    } else if (odd_degs == 2) {
        start = std::find_if(g.adj.begin(), g.adj.end(), [](auto &v) { return v.size() % 2 != 0; }) - g.adj.begin();
    }

    if (start == g.n) {
        return std::nullopt;
    }

    std::vector<int> deg(g.n), ind(g.n);
    for (int i = 0; i < g.n; i++) {
        deg[i] = ind[i] = g.adj[i].size();
    }

    std::vector<int> st;
    st.reserve(g.m + 1);
    st.push_back(start);

    std::vector<int> circuit;
    circuit.reserve(g.m);

    std::vector<bool> mark(g.m);
    while (!st.empty()) {
        int u = st.back();
        if (!deg[u]) {
            st.pop_back();
            if (!st.empty()) {
                int i = st.back();
                int v = u ^ g.edges[i].u ^ g.edges[i].v;
                st.back() = v;
                circuit.push_back(i);
            }
        } else {
            while (mark[g.adj[u][--ind[u]]])
                ;
            int i = g.adj[u][ind[u]];
            mark[i] = true;
            int v = u ^ g.edges[i].u ^ g.edges[i].v;
            deg[u]--, deg[v]--;
            st.back() = i;
            st.push_back(v);
        }
    }

    if ((int)circuit.size() != g.m) {
        return std::nullopt;
    }

    std::reverse(circuit.begin(), circuit.end());

    return trail{
        .g = g,
        .start = start,
        .edges = circuit,
    };
}

} // namespace graphs
