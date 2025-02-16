#pragma once

#include "../../algebra/concepts.hpp"
#include "../../range_query/concepts.hpp"
#include "../heavy_light_decomposition.hpp"
#include "dynamic_path_query.hpp"
#include <iterator>

namespace trees {

template <typename Graph, range_query::RangeQuery Rq>
    requires algebra::Commutative<typename Rq::groupoid>
struct dynamic_path_query<Graph, Rq> {
    using groupoid = typename Rq::groupoid;
    using value_type = typename groupoid::value_type;

    groupoid g;
    const heavy_light_decomposition<Graph> &hld;
    Rq rq;

    template <std::forward_iterator I>
    static std::vector<value_type> build_values(const heavy_light_decomposition<Graph> &hld, I first, I last) {
        int n = hld.g.n;
        std::vector<value_type> values(n);
        for (int u = 0; u < n; u++) {
            values[hld.tree_pos[u]] = value_type(*first++);
        }
        return values;
    }

    dynamic_path_query(groupoid g, const heavy_light_decomposition<Graph> &hld, const std::vector<value_type> &values) : g(g), hld(hld), rq(values.begin(), values.end()) {}

    template <std::forward_iterator I>
    dynamic_path_query(groupoid g, const heavy_light_decomposition<Graph> &hld, I first, I last) : dynamic_path_query(g, hld, build_values(hld, first, last)) {}

    template <std::forward_iterator I>
    dynamic_path_query(const heavy_light_decomposition<Graph> &hld, I first, I last) : dynamic_path_query(groupoid(), hld, build_values(hld, first, last)) {}

    value_type path_query(int u, int v) {
        value_type ans = g.id();
        hld.unordered_visit_path(u, v, [&](int l, int r) {
            ans = g.op(ans, rq.range_query(l, r));
        });
        return ans;
    }

    template <typename T, typename... Args, typename... ExtraArgs>
    void vertex_update(void (T::*update)(int, Args...), int u, ExtraArgs &&...extra_args) {
        (rq.*update)(hld.tree_pos[u], std::forward<ExtraArgs>(extra_args)...);
    }

    template <typename T, typename... Args, typename... ExtraArgs>
    void path_update(void (T::*update)(int, int, Args...), int u, int v, ExtraArgs &&...extra_args) {
        hld.unordered_visit_path(u, v, [&](int l, int r) {
            (rq.*update)(l, r, std::forward<ExtraArgs>(extra_args)...);
        });
    }
};

} // namespace trees
