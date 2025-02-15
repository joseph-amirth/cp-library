#pragma once

#include "directed_graph.hpp"
#include "strong_connectivity/tarjan.hpp"

std::vector<bool> two_sat(int n, const std::vector<std::pair<int, int>> &clauses) {
    graph::directed_graph<> g(2 * n, 2 * clauses.size());
    for (auto &[x, y] : clauses) {
        g.add_edge(x < n ? x + n : x - n, y);
        g.add_edge(y < n ? y + n : y - n, x);
    }

    auto sccs = graph::tarjan_sccs(g);

    std::vector<int> t(2 * n);
    for (int i = 0; i < (int)sccs.size(); i++) {
        for (int x : sccs[i]) {
            t[x] = i;
        }
    }

    std::vector<bool> assignment(n);
    for (int i = 0; i < n; i++) {
        if (t[i] == t[i + n]) {
            return {};
        }
        assignment[i] = t[i] > t[i + n];
    }
    return assignment;
}
