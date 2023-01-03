#pragma once

#include "dynamic-path-query.hpp"
#include "../../algebra/type-traits.hpp"

namespace trees {

template <typename Graph, typename RangeQuery>
struct dynamic_path_query<Graph, RangeQuery,
        std::enable_if_t<algebra::is_commutative_v<typename RangeQuery::groupoid>>,
        std::enable_if_t<algebra::is_group_v<typename RangeQuery::groupoid>>> {

    // TODO(implement dis)
};

}
