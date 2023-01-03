#pragma once

#include "basic.hpp"
#include "../../algebra/groupoid/merge.hpp"

#include <vector>
#include <algorithm>

namespace data_structures {

template <typename Value, typename Container = std::vector<Value>, typename Compare = std::less<>>
struct merge_sort_tree : data_structures::segment_tree<algebra::merge_monoid<Container, Compare>> {
    using value_type = Value;
    using container_type = Container;
    using compare_type = Compare;

    using groupoid = algebra::merge_monoid<container_type, compare_type>;

    using data_structures::segment_tree<groupoid>::n;
    using data_structures::segment_tree<groupoid>::t;
    using data_structures::segment_tree<groupoid>::build;

    merge_sort_tree() {}

    template <typename iterator_t>
    merge_sort_tree(iterator_t first, iterator_t last, compare_type compare = compare_type())
            : data_structures::segment_tree<groupoid>(std::distance(first, last), groupoid(compare)) {
        build([&first](container_type &leaf) {
            leaf = {value_type(*first)};
            ++first;
        });
    }

    int range_rank(int l, int r, const value_type &x) {
        int rank = 0;
        data_structures::segment_tree<groupoid>::template split_range(l, r, [&x, &rank, this](int i, ...) {
            const container_type &c = t[i];
            rank += std::distance(std::begin(c), std::lower_bound(std::begin(c), std::end(c), x));
        });
        return rank;
    }

    int range_frequency(int l, int r, const value_type &x) {
        int frequency = 0;
        data_structures::segment_tree<groupoid>::template split_range(l, r, [&x, &frequency, this](int i, ...) {
            const container_type &c = t[i];
            auto [lb, ub] = std::equal_range(std::begin(c), std::end(c), x);
            frequency += std::distance(lb, ub);
        });
        return frequency;
    }
};

}
