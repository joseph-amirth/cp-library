#pragma once

#include <type_traits>

template<typename mint>
struct is_static_mint_helper : std::false_type {
};

template<typename mint>
struct is_static_mint : is_static_mint_helper<typename std::decay<mint>::type> {
};

template<typename mint>
struct is_dynamic_mint_helper : std::false_type {
};

template<typename mint>
struct is_dynamic_mint : is_dynamic_mint_helper<typename std::decay<mint>::type> {
};

template<typename mint>
struct is_mint : std::integral_constant<bool, is_static_mint<mint>::value || is_dynamic_mint<mint>::value> {
};