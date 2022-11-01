#pragma once

#include "basic.hpp"
#include "../../algebra/groupoid/merge.hpp"

#include <vector>
#include <algorithm>

namespace data_structures {

template <typename T, typename C>
struct merge_sort_tree : data_structures::segment_tree<algebra::merge_monoid<C>> {
    using value_type = T;
    using container_type = C;
    using monoid_type = algebra::merge_monoid<C>;
    using base_class_type = data_structures::segment_tree<algebra::merge_monoid<C>>;

    using base_class_type::n;
    using base_class_type::t;
    using base_class_type::build;

    merge_sort_tree() : base_class_type() {}

    template <typename iterator_t>
    merge_sort_tree(iterator_t first, iterator_t last) {
        n = std::distance(first, last);
        t.assign(4 * n, monoid_type::e());
        build([&first](container_type &leaf) {
            leaf = {value_type(*first)};
            ++first;
        });
    }

    int range_rank(int l, int r, const value_type &x) {
        int order = 0;
        base_class_type::template split_range(l, r, [&x, &order](const container_type &c) {
            order += std::distance(std::begin(c), std::lower_bound(std::begin(c), std::end(c), x));
        });
        return order;
    }
};

}
