#pragma once

#include "algebra/monoids/merge.hpp"
#include "basic.hpp"

#include <algorithm>
#include <concepts>
#include <iterator>
#include <ranges>
#include <vector>

namespace range_query {

template <typename T, std::ranges::range R = std::vector<T>, std::strict_weak_order<T, T> C = std::less<>>
struct merge_sort_tree : segment_tree<algebra::merge<R, C>> {
    using M = algebra::merge<R, C>;

    using segment_tree<M>::n;
    using segment_tree<M>::t;
    using segment_tree<M>::build;

    template <std::forward_iterator I>
    merge_sort_tree(C compare, I first, I last) : segment_tree<M>(M(compare), std::distance(first, last)) {
        build([&first](R &leaf) {
            leaf = {T(*first)};
            ++first;
        });
    }

    template <std::forward_iterator I>
    merge_sort_tree(I first, I last) : merge_sort_tree(C(), first, last) {}

    int range_rank(int l, int r, const T &x) {
        int rank = 0;
        segment_tree<M>::template split_range(l, r, [&x, &rank, this](int i, ...) {
            const R &c = t[i];
            rank += std::distance(std::begin(c), std::lower_bound(std::begin(c), std::end(c), x));
        });
        return rank;
    }

    int range_frequency(int l, int r, const T &x) {
        int frequency = 0;
        segment_tree<M>::template split_range(l, r, [&x, &frequency, this](int i, ...) {
            const R &c = t[i];
            auto [lb, ub] = std::equal_range(std::begin(c), std::end(c), x);
            frequency += std::distance(lb, ub);
        });
        return frequency;
    }
};

} // namespace range_query
