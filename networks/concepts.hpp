#pragma once

#include "type_traits.hpp"

namespace networks {

template <typename Edge>
concept EdgeWithCost = is_edge_with_cost_v<Edge>;

} // namespace networks
