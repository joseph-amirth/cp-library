#pragma once

#include "../../algebra/type-traits.hpp"
#include "../euler-tour-tree.hpp"
#include "dynamic_path_query.hpp"

namespace trees {

template <typename Graph, typename RangeQuery>
struct dynamic_path_query<Graph, RangeQuery,
                          std::enable_if_t<algebra::is_commutative_v<typename RangeQuery::groupoid>>,
                          std::enable_if_t<algebra::is_group_v<typename RangeQuery::groupoid>>> {

    using graph_type = Graph;
    using range_query_type = RangeQuery;

    using groupoid = typename range_query_type::groupoid;
    using value_type = typename groupoid::value_type;

    const euler_tour_tree<graph_type> &ett;
    range_query_type rq;

    template <typename U>
    dynamic_path_query(const euler_tour_tree<graph_type> &ett, const std::vector<U> &a) : ett(ett) {
        int n = ett.g.n;
        std::vector<value_type> values(2 * n);
        for (int u = 0; u < n; u++) {
            values[ett.tin[u]] = a[u];
            values[ett.tout[u]] = groupoid::inv(a[u]);
        }
        rq = range_query_type(values.begin(), values.end());
    }

    value_type path_query(int u, int v) {
        int root = ett.euler_tour.front();
    }

    template <typename T, typename... Args, typename... ExtraArgs>
    void vertex_update(void (T::*tin_update)(int, Args...), void (T::*tout_update)(int, Args...), int u, ExtraArgs &&...extra_args) {
        (rq.*tin_update)(ett.tin[u], std::forward<ExtraArgs>(extra_args)...);
        (rq.*tout_update)(ett.tout[u], std::forward<ExtraArgs>(extra_args)...);
    }

    template <typename T, typename... Args, typename... ExtraArgs>
    void path_update(void (T::*update)(int, int, Args...), int u, int v, ExtraArgs &&...extra_args) {
        // TODO: Figure out how to support this.
    }
};

} // namespace trees
