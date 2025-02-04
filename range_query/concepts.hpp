#pragma once

#include <concepts>

namespace range_query {

template <typename Rq>
concept RangeQuery = requires(Rq rq, std::size_t l, std::size_t r) {
    { rq.range_query(l, r) } -> std::same_as<typename Rq::value_type>;
};

} // namespace range_query
