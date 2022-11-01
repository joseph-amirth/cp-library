#pragma once

#include <type_traits>

namespace algebra {

template <typename F>
struct function_composition_monoid {
    using value_type = F;
    using is_commutative = std::false_type;

    static value_type e() {
        return value_type();
    }

    static value_type op(const value_type &f, const value_type &g) {
        return g(f);
    }
};

}