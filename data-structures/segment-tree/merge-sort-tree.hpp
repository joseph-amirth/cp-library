#pragma once

#include "basic.hpp"
#include "../../algebra/groupoid/merge.hpp"

#include <vector>
#include <algorithm>

namespace data_structures {

template <typename Value, typename Container = std::vector<Value>>
struct merge_sort_tree : data_structures::segment_tree<algebra::merge_monoid<Container>> {
    using value_type = Value;
    using container_type = Container;
    using groupoid_type = algebra::merge_monoid<Container>;
    using base_class_type = data_structures::segment_tree<algebra::merge_monoid<Container>>;

    using base_class_type::n;
    using base_class_type::t;
    using base_class_type::build;

    merge_sort_tree() : base_class_type() {}

    template <typename iterator_t>
    merge_sort_tree(iterator_t first, iterator_t last) {
        n = std::distance(first, last);
        t.assign(4 * n, groupoid_type::e());
        build([&first](container_type &leaf) {
            leaf = {value_type(*first)};
            ++first;
        });
    }

    int range_rank(int l, int r, const value_type &x) {
        int rank = 0;
        base_class_type::template split_range(l, r, [&x, &rank, this](int i, ...) {
            const container_type &c = t[i];
            rank += std::distance(std::begin(c), std::lower_bound(std::begin(c), std::end(c), x));
        });
        return rank;
    }

    int range_frequency(int l, int r, const value_type &x) {
        int frequency = 0;
        base_class_type::template split_range(l, r, [&x, &frequency, this](int i, ...) {
            const container_type &c = t[i];
            auto [lb, ub] = std::equal_range(std::begin(c), std::end(c), x);
            frequency += std::distance(lb, ub);
        });
        return frequency;
    }
};

}
