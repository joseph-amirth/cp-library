#pragma once

#include "../range_query/concepts.hpp"
#include "../tree/euler_tour_tree.hpp"

namespace tree {

template <typename G, range_query::RangeQuery Rq>
struct dynamic_subtree_query {
    using value_type = typename Rq::value_type;

    const compressed_euler_tour_tree<G> &et;

    Rq rq;

    template <typename T>
    static Rq build_rq(const compressed_euler_tour_tree<G> &et, const std::vector<T> &a) {
        std::vector<value_type> values(et.g.n);
        for (std::size_t u = 0; u < et.g.n; u++) {
            values[et.tin[u]] = value_type(a[u]);
        }
        return Rq(values.begin(), values.end());
    }

    template <typename T>
    dynamic_subtree_query(const compressed_euler_tour_tree<G> &et, const std::vector<T> &a) : et(et), rq(build_rq(et, a)) {}

    value_type subtree_query(std::size_t u) {
        return rq.range_query(et.tin[u], et.tout[u]);
    }

    template <typename T, typename... Args, typename... ExtraArgs>
    void vertex_update(void (T::*update)(std::size_t, Args...), std::size_t u, ExtraArgs &&...extra_args) {
        (rq.*update)(et.tin[u], std::forward<ExtraArgs>(extra_args)...);
    }

    template <typename T, typename... Args, typename... ExtraArgs>
    void subtree_update(void (T::*update)(std::size_t, std::size_t, Args...), std::size_t u, ExtraArgs &&...extra_args) {
        (rq.*update)(et.tin[u], et.tout[u], std::forward<ExtraArgs>(extra_args)...);
    }
};

} // namespace tree
