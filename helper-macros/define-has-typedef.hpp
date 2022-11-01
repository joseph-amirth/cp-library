#pragma once

#include <type_traits>

#define DEFINE_HAS_TYPEDEF(name)                                    \
template <typename T, typename = void>                              \
struct has_typedef_##name : std::false_type {};                     \
                                                                    \
template <typename T>                                               \
struct has_typedef_##name<T, std::void_t<typename T::name>> : std::true_type {};
