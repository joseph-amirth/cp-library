#pragma once

#include "dynamic-path-query.hpp"
#include "../../algebra/type-traits.hpp"
#include "../heavy-light-decomposition.hpp"

namespace trees {

template <typename Graph, typename RangeQuery>
struct dynamic_path_query<Graph, RangeQuery,
        std::enable_if_t<!algebra::is_commutative_v<typename RangeQuery::monoid_type>>, void> {

    using graph_type = Graph;
    using range_query_type = RangeQuery;

    using monoid_type = typename range_query_type::monoid_type;
    using value_type = typename monoid_type::value_type;

    const heavy_light_decomposition<graph_type> &hld;
    range_query_type rq, reverse_rq;

    template <typename U>
    dynamic_path_query(const heavy_light_decomposition<graph_type> &hld, const std::vector<U> &a) : hld(hld) {
        int n = (int) hld.parent.size();
        std::vector<value_type> values(n);
        for (int u = 0; u < n; u++) {
            values[hld.tree_pos[u]] = value_type(a[u]);
        }
        rq = range_query_type(values.begin(), values.end());
        reverse_rq = range_query_type(values.rbegin(), values.rend());
    }

    value_type path_query(int u, int v) {
        value_type ans_up = monoid_type::e();
        value_type ans_down = monoid_type::e();
        hld.semiordered_visit_path(u, v, [&](int l, int r) {
            ans_up = monoid_type::op(ans_up, rq.range_query(l, r));
        }, [&](int l, int r) {
            ans_down = monoid_type::op(reverse_rq.range_query(hld.g.n - 1 - r, hld.g.n - 1 - l), ans_down);
        });
        return monoid_type::op(ans_up, ans_down);
    }

    template <typename T, typename...args>
    void vertex_update(void (T::*update)(int, args...), int u, auto &&...extra_args) {
        (rq.*update)(hld.tree_pos[u], std::forward<decltype(extra_args)>(extra_args)...);
        (reverse_rq.*update)(hld.g.n - 1 - hld.tree_pos[u], std::forward<decltype(extra_args)>(extra_args)...);
    }

    template <typename T, typename...args>
    void path_update(void (T::*update)(int, int, args...), int u, int v, auto &&...extra_args) {
        hld.unordered_visit_path(u, v, [&](int l, int r) {
            (rq.*update)(l, r, std::forward<decltype(extra_args)>(extra_args)...);
            (reverse_rq.*update)(hld.g.n - 1 - r, hld.g.n - 1 - l, std::forward<decltype(extra_args)>(extra_args)...);
        });
    }
};

}
