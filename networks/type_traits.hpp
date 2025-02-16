#pragma once

#include <type_traits>

namespace networks {

template <typename Edge>
using flow_t = typename Edge::flow_type;

template <typename Edge>
using cost_t = typename Edge::cost_type;

template <typename Edge>
struct is_edge_with_cost : std::false_type {};

template <typename Edge>
constexpr bool is_edge_with_cost_v = is_edge_with_cost<Edge>::value;

} // namespace networks
