#pragma once

#include "canonical-rng.hpp"

namespace algebra {

template <typename T>
struct canonical_ring : canonical_rng<T> {
    using value_type = T;

    static value_type unit() {
        return T(1);
    }
};

}
