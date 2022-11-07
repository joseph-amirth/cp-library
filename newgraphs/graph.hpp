#pragma once

#include <vector>

namespace graphs {

template <typename Edge>
struct graph {
    using edge_type = Edge;

    int n, m;
    std::vector<edge_type> edges;
    std::vector<std::vector<int>> adj;

    graph() : n(), m() {}

    graph(int n, int expected_m = 0) : n(n), m(), adj(n) {
        edges.reserve(expected_m);
    }

    virtual int add_edge(edge_type &&e) = 0;

    template <typename...Args>
    int add_edge(int u, int v, Args&&...args) {
        return add_edge(edge_type(u, v, std::forward<Args>(args)...));
    }
};

template <typename Graph>
using edge_t = typename Graph::edge_type;

}
