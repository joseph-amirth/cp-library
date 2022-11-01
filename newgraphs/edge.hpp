#pragma once

namespace graphs {

struct edge {
    int u, v;

    edge() : u(-1), v(-1) {}

    edge(int u, int v) : u(u), v(v) {}
};

template <typename Weight>
struct weighted_edge : edge {
    using weight_type = Weight;

    weight_type w;

    weighted_edge() : edge() {}

    weighted_edge(int u, int v, weight_type w) : edge(u, v), w(w) {}
};

}
