#pragma once

#include <type_traits>

namespace graphs {

template <typename Graph>
struct is_weighted : std::false_type {};

template <typename Graph>
constexpr bool is_weighted_v = is_weighted<Graph>::value;

template <typename Graph>
concept WeightedGraph = is_weighted_v<Graph>;

} // namespace graphs
