#pragma once

#include <type_traits>

#include "../helper-macros/define-has-typedef.hpp"
#include "../helper-macros/define-has-static-method.hpp"

namespace algebra {

DEFINE_HAS_TYPEDEF(value_type)
DEFINE_HAS_TYPEDEF(is_commutative)
DEFINE_HAS_TYPEDEF(is_idempotent)

DEFINE_HAS_STATIC_METHOD(e)
DEFINE_HAS_STATIC_METHOD(op)
DEFINE_HAS_STATIC_METHOD(inv)

template <typename T>
using element_t = typename T::value_type;

template <typename T>
struct is_semigroup :
        std::conjunction<
            has_typedef_value_type<T>,
            has_typedef_is_commutative<T>,
            has_static_method_op<T, element_t<T>(const element_t<T>&, const element_t<T>&)>> {};

template <typename T>
constexpr bool is_semigroup_v = is_semigroup<T>::value;

template <typename T>
struct is_monoid :
        std::conjunction<
            is_semigroup<T>,
            has_static_method_e<T, element_t<T>(void)>> {};

template <typename T>
constexpr bool is_monoid_v = is_monoid<T>::value;

template <typename T>
struct is_group :
        std::conjunction<
            is_monoid<T>,
            has_static_method_inv<T, element_t<T>(const element_t<T>&)>> {};

template <typename T>
constexpr bool is_group_v = is_group<T>::value;

template <typename T>
struct is_commutative : T::is_commutative {};

template <typename T>
constexpr bool is_commutative_v = is_commutative<T>::value;

template <typename T, typename = void>
struct is_idempotent : std::false_type {};

template <typename T>
struct is_idempotent<T, std::void_t<typename T::is_idempotent>> : T::is_idempotent {};

template <typename T>
constexpr bool is_idempotent_v = is_idempotent<T>::value;

}
