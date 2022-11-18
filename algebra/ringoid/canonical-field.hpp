#pragma once

#include "canonical-ring.hpp"
#include "../../miscellaneous/inv.hpp"

namespace algebra {

template <typename T>
struct canonical_field : canonical_ring<T> {
    using value_type = T;
    using is_commutative = std::true_type;

    static value_type reciprocal(const value_type &x) {
        return inv(x);
    }
};

}
