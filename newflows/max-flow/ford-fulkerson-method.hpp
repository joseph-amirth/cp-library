#include "../network.hpp"
#include "../primitives/augment-path.hpp"

namespace networks {

template <typename Edge>
typename Edge::flow_type ford_fulkerson_method(network<Edge> &g, int s, int t, typename Edge::flow_type target_flow, auto &&f) {
    using edge_type = Edge;
    using flow_type = typename edge_type::flow_type;

    std::vector<int> p(g.n);
    flow_type flow = 0;
    while (flow < target_flow && f(p)) {
        flow += augment_path(g, s, t, p, target_flow - flow);
    }
    return flow;
}

}
