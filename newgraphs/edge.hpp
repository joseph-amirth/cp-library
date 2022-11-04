#pragma once

namespace graphs {

struct edge {
    int u, v;

    edge() : u(-1), v(-1) {}

    edge(int u, int v) : u(u), v(v) {}
};

template <typename Weight, typename WeightSum = Weight>
struct weighted_edge : edge {
    using weight_type = Weight;
    using weight_sum_type = WeightSum;

    weight_type w;

    weighted_edge() : edge() {}

    weighted_edge(int u, int v, weight_type w) : edge(u, v), w(w) {}
};

template <typename Edge>
using weight_t = typename Edge::weight_type;

template <typename Edge>
using weight_sum_t = typename Edge::weight_sum_type;

}
