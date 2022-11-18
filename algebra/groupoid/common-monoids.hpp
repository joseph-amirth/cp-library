#pragma once

#include <type_traits>

namespace algebra {

#define DEFINE_MONOID_FROM_OP(name, oper, id, commutative)              \
template <typename T>                                                   \
struct name##_monoid {                                                  \
    using value_type = T;                                               \
    using is_commutative = std:: commutative##_type;                    \
                                                                        \
    static value_type e() {                                             \
        return value_type(id);                                          \
    }                                                                   \
                                                                        \
    static value_type op(const value_type &x, const value_type &y) {    \
        return x oper y;                                                \
    }                                                                   \
};

DEFINE_MONOID_FROM_OP(sum, +, 0, true)
DEFINE_MONOID_FROM_OP(product, *, 1, true)
DEFINE_MONOID_FROM_OP(and, &, -1, true)
DEFINE_MONOID_FROM_OP(or, |, 0, true)
DEFINE_MONOID_FROM_OP(xor, ^, 0, true)

}
