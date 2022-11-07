#pragma once

#include "dynamic-path-query.hpp"
#include "../../algebra/type-traits.hpp"

namespace trees {

template <typename G, typename R>
struct dynamic_path_query<G, R,
        std::enable_if_t<algebra::is_commutative_v<typename R::monoid_type>>,
        std::enable_if_t<algebra::is_group_v<typename R::monoid_type>>> {

    // TODO(implement dis)
};

}
