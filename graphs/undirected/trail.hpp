#pragma once

#include "graphs/undirected_graph.hpp"

#include <ranges>

namespace graphs {

template <typename Edge>
struct trail {
    const undirected_graph<Edge> &g;

    int start;
    std::vector<int> edges;

    std::ranges::range auto vertices() {
        auto next = [this, u = start](int i) mutable {
            if (i == 0) {
                return u;
            } else {
                int j = edges[i - 1];
                int v = g.edges[j].u ^ g.edges[j].v ^ u;
                return u = v;
            }
        };

        return std::views::iota(0) | std::views::take(edges.size() + 1) | std::views::transform(next);
    }
};

} // namespace graphs
