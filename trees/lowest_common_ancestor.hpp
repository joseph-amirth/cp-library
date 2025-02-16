#pragma once

#include "../algebra/semigroups/extremum_index.hpp"
#include "../range_query/sparse_table.hpp"
#include "euler_tour_tree.hpp"

namespace trees {

namespace {
using min_tin = algebra::semigroups::extremum_index<int>;
}

template <typename Graph>
struct lowest_common_ancestor {
    const euler_tour_tree<Graph> &ett;
    range_query::sparse_table<min_tin> st;

    lowest_common_ancestor(const euler_tour_tree<Graph> &ett) : ett(ett), st(min_tin(ett.tin), ett.euler_tour.begin(), ett.euler_tour.end()) {}

    int lca(int u, int v) {
        return ett.tin[u] < ett.tin[v] ? st.range_query(ett.tin[u], ett.tin[v]) : st.range_query(ett.tin[v], ett.tin[u]);
    }
};

} // namespace trees
