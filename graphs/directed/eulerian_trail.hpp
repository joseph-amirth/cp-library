#pragma once

#include "graphs/directed_graph.hpp"
#include "trail.hpp"

#include <algorithm>
#include <cassert>
#include <optional>
#include <ranges>

namespace graphs {

template <typename Edge>
std::optional<trail<Edge>> find_eulerian_trail(const directed_graph<Edge> &g) {
    std::vector<int> in(g.n), out(g.n);
    for (auto [u, v] : g.edges) {
        out[u]++, in[v]++;
    }

    int uneven = 0;
    for (int u = 0; u < g.n; u++) {
        uneven += in[u] != out[u];
    }

    int start;
    if (uneven == 0) {
        start = 0;
        for (int u = 0; u < g.n; u++) {
            if (in[u] != 0) {
                start = u;
                break;
            }
        }
    } else if (uneven == 2) {
        int v = -1;
        for (int u = 0; u < g.n; u++) {
            if (out[u] > in[u]) {
                v = u;
                break;
            }
        }
        assert(v != -1);
        if (out[v] != 1 + in[v]) {
            return std::nullopt;
        }
        start = v;
    } else {
        return std::nullopt;
    }

    std::vector<int> ind = out;

    std::vector<int> st;
    st.reserve(g.m + 1);
    st.push_back(start);

    std::vector<int> circuit;
    circuit.reserve(g.m);

    std::vector<bool> mark(g.m);
    while (!st.empty()) {
        int u = st.back();
        if (!out[u]) {
            st.pop_back();
            if (!st.empty()) {
                int i = st.back();
                int u = g.edges[i].u;
                st.back() = u;
                circuit.push_back(i);
            }
        } else {
            while (mark[g.adj[u][--ind[u]]])
                ;
            int i = g.adj[u][ind[u]];
            mark[i] = true;
            int v = g.edges[i].v;
            out[u]--;
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
