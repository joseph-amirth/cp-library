#pragma once

#include "algebra/concepts.hpp"
#include <concepts>
#include <utility>

namespace range_query {

template <typename Rq>
concept RangeQuery = requires(Rq rq, std::size_t l, std::size_t r) {
    { rq.range_query(l, r) } -> std::same_as<typename Rq::value_type>;
};

template <typename Act>
concept LazyAction =
    algebra::Monoid<Act> &&
    requires(Act act, typename Act::value_type f, typename Act::target_type x, int size) {
        { act.apply(f, x, size) } -> std::same_as<typename Act::target_type>;
        { act.split(f, size) } -> std::same_as<std::pair<typename Act::value_type, typename Act::value_type>>;
    };

} // namespace range_query
