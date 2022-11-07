#pragma once

#include "../newgraphs/undirected-graph.hpp"
#include "../trees/euler-tour-tree.hpp"
#include <functional>

namespace trees {

template <typename Graph, typename RangeQuery>
struct dynamic_subtree_query {
    using graph_type = Graph;
    using range_query_type = RangeQuery;
    using value_type = typename range_query_type::value_type;

    const compressed_euler_tour_tree<graph_type> &et;

    range_query_type rq;

    template <typename U>
    dynamic_subtree_query(const compressed_euler_tour_tree<graph_type> &et, const std::vector<U> &a) : et(et) {
        std::vector<value_type> values(et.g.n);
        for (int u = 0; u < et.g.n; u++) {
            values[et.tin[u]] = value_type(a[u]);
        }
        rq = range_query_type(values.begin(), values.end());
    }

    value_type subtree_query(int u) {
        return rq.range_query(et.tin[u], et.tout[u]);
    }

    template <typename T, typename...Args, typename...ExtraArgs>
    void vertex_update(void (T::*update)(int, Args...), int u, ExtraArgs &&...extra_args) {
        (rq.*update)(et.tin[u], std::forward<ExtraArgs>(extra_args)...);
    }

    template <typename T, typename...Args, typename...ExtraArgs>
    void subtree_update(void (T::*update)(int, int, Args...), int u, ExtraArgs &&...extra_args) {
        (rq.*update)(et.tin[u], et.tout[u], std::forward<ExtraArgs>(extra_args)...);
    }
};

}
