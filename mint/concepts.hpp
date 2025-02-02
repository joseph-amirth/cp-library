#pragma once

#include <type_traits>

namespace _detail {

template <typename Mint>
struct is_static_mint_helper : std::false_type {};

template <typename Mint>
struct is_static_mint : is_static_mint_helper<std::decay_t<Mint>> {};

template <typename Mint>
inline constexpr bool is_static_mint_v = is_static_mint<Mint>::value;

template <typename Mint>
struct is_dynamic_mint_helper : std::false_type {};

template <typename Mint>
struct is_dynamic_mint : is_dynamic_mint_helper<std::decay_t<Mint>> {};

template <typename Mint>
inline constexpr bool is_dynamic_mint_v = is_dynamic_mint<Mint>::value;

} // namespace _detail

template <typename Mint>
concept StaticMint = _detail::is_static_mint_v<Mint>;

template <typename Mint>
concept DynamicMint = _detail::is_dynamic_mint_v<Mint>;
