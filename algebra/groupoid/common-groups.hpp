#pragma once

#include "common-monoids.hpp"

namespace algebra {

#define DEFINE_GROUP_FROM_MONOID_AND_INVERSE_OP(name, inv_op)           \
template <typename T>                                                   \
struct name##_group : name##_monoid<T> {                                \
    using typename name##_monoid<T>::value_type;                        \
    using name##_monoid<T>::e;                                          \
                                                                        \
    static value_type inv(const value_type &x) {                        \
        return e() inv_op x;                                            \
    }                                                                   \
};

DEFINE_GROUP_FROM_MONOID_AND_INVERSE_OP(sum, -)
DEFINE_GROUP_FROM_MONOID_AND_INVERSE_OP(product, /)
DEFINE_GROUP_FROM_MONOID_AND_INVERSE_OP(xor, ^)

}
