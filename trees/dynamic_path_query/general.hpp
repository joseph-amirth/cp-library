#pragma once

#include "../../algebra/type-traits.hpp"
#include "../heavy-light-decomposition.hpp"
#include "dynamic_path_query.hpp"

namespace trees {

template <typename Graph, typename RangeQuery>
struct dynamic_path_query<Graph, RangeQuery,
                          std::enable_if_t<!algebra::is_commutative_v<typename RangeQuery::groupoid>>, void> {

    using graph_type = Graph;
    using range_query_type = RangeQuery;

    using groupoid = typename range_query_type::groupoid;
    using value_type = typename groupoid::value_type;

    const heavy_light_decomposition<graph_type> &hld;
    range_query_type rq, reverse_rq;

    template <typename U>
    dynamic_path_query(const heavy_light_decomposition<graph_type> &hld, const std::vector<U> &a) : hld(hld) {
        int n = (int)hld.parent.size();
        std::vector<value_type> values(n);
        for (int u = 0; u < n; u++) {
            values[hld.tree_pos[u]] = value_type(a[u]);
        }
        rq = range_query_type(values.begin(), values.end());
        reverse_rq = range_query_type(values.rbegin(), values.rend());
    }

    value_type path_query(int u, int v) {
        value_type ans_up = groupoid::e();
        value_type ans_down = groupoid::e();
        hld.semiordered_visit_path(
            u, v, [&](int l, int r) { ans_up = groupoid::op(ans_up, rq.range_query(l, r)); }, [&](int l, int r) { ans_down = groupoid::op(reverse_rq.range_query(hld.g.n - 1 - r, hld.g.n - 1 - l), ans_down); });
        return groupoid::op(ans_up, ans_down);
    }

    template <typename T, typename... Args, typename... ExtraArgs>
    void vertex_update(void (T::*update)(int, Args...), int u, ExtraArgs &&...extra_args) {
        (rq.*update)(hld.tree_pos[u], std::forward<ExtraArgs>(extra_args)...);
        (reverse_rq.*update)(hld.g.n - 1 - hld.tree_pos[u], std::forward<ExtraArgs>(extra_args)...);
    }

    template <typename T, typename... Args, typename... ExtraArgs>
    void path_update(void (T::*update)(int, int, Args...), int u, int v, ExtraArgs &&...extra_args) {
        hld.unordered_visit_path(u, v, [&](int l, int r) {
            (rq.*update)(l, r, std::forward<ExtraArgs>(extra_args)...);
            (reverse_rq.*update)(hld.g.n - 1 - r, hld.g.n - 1 - l, std::forward<ExtraArgs>(extra_args)...);
        });
    }
};

} // namespace trees
