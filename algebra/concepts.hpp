#pragma once

#include <concepts>
#include <type_traits>

namespace algebra {

template <typename S>
concept Semigroup = requires(S m, typename S::value_type a, typename S::value_type b) {
    { m.op(a, b) } -> std::same_as<typename S::value_type>;
};

template <typename M>
concept Monoid = Semigroup<M> && requires(M m, typename M::value_type a, typename M::value_type b) {
    { m.id() } -> std::same_as<typename M::value_type>;
};

template <typename G>
concept Group = Monoid<G> && requires(G m, typename G::value_type a) {
    { m.inv(a) } -> std::same_as<typename G::value_type>;
};

template <typename S>
struct is_commutative : std::false_type {};

template <typename S>
inline constexpr bool is_commutative_v = is_commutative<S>::value;

template <typename S>
concept Commutative = is_commutative_v<S>;

template <typename S>
struct is_idempotent : std::false_type {};

template <typename S>
inline constexpr bool is_idempotent_v = is_idempotent<S>::value;

template <typename S>
concept Idempotent = is_idempotent_v<S>;

} // namespace algebra
