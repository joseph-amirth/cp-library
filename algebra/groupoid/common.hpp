#pragma once

#include <type_traits>

namespace algebra {

#define DEFINE_MONOID_FROM_OP(name, oper, id, commutative)              \
template <typename T>                                                   \
struct name##_monoid {                                                  \
    using value_type = T;                                             \
    using is_commutative = std:: commutative##_type;                    \
                                                                        \
    static value_type e() {                                           \
        return value_type(id);                                        \
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

#define DEFINE_GROUP_FROM_MONOID_AND_INVERSE_OP(name, inv_op)           \
template <typename T>                                                   \
struct name##_group : name##_monoid<T> {                                \
    using typename name##_monoid<T>::value_type;                      \
    using name##_monoid<T>::e;                                          \
                                                                        \
    static value_type inv(const value_type &x) {                    \
        return e() inv_op x;                                            \
    }                                                                   \
};

DEFINE_GROUP_FROM_MONOID_AND_INVERSE_OP(sum, -)
DEFINE_GROUP_FROM_MONOID_AND_INVERSE_OP(product, /)
DEFINE_GROUP_FROM_MONOID_AND_INVERSE_OP(xor, ^)

}
