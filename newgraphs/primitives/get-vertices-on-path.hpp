#pragma once

#include "../graph.hpp"

namespace graphs {

template <typename Graph>
std::vector<int> get_vertices_on_path(const Graph &g, int s, int t, auto &&p) {
    std::vector<int> path;
    for (int u = t; u != s; ) {
        path.push_back(u);
        const auto &e = g.edges[p(u)];
        u ^= e.u ^ e.v;
    }
    path.push_back(s);
    std::reverse(path.begin(), path.end());
    return path;
}

template <typename Graph>
std::vector<int> get_vertices_on_path(const Graph &g, int s, int t, const std::vector<int> &p) {
    return get_vertices_on_path(g, s, t, [&p](int u) {
        return p[u];
    });
}

}