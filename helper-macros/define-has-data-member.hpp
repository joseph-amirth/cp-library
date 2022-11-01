#pragma once

#include <type_traits>

#define DEFINE_HAS_DATA_MEMBER(name)                                \
template <typename T, typename U, typename = void>                  \
struct has_data_member_##name : std::false_type {};                 \
                                                                    \
template <typename T, typename U>                                   \
struct has_data_member_##name<T, U, std::void_t<decltype(declval<T>().name)>> : std::is_same<U, decltype(declval<T>().name)> {};
