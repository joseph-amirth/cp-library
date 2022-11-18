#pragma once

#include <type_traits>

#define DEFINE_HAS_METHOD(name)                                     \
template <typename T, typename Ret, typename...Args>                \
struct has_method_ ##name## _helper;                                \
                                                                    \
template <typename T, typename Ret, typename... Args>               \
struct has_method_ ##name## _helper<T, Ret(Args...)> {              \
    template<typename C>                                            \
    static constexpr auto check(C*) -> typename std::is_same<Ret, decltype(std::declval<C>(). name (std::declval<Args>()...))>::type;   \
                                                                    \
    template<typename>                                              \
    static constexpr std::false_type check(...);                    \
                                                                    \
    using type = decltype(check<T>(0));                             \
};                                                                  \
                                                                    \
template <typename T, typename Return, typename...Args>             \
struct has_method_##name;                                           \
                                                                    \
template <typename T, typename Return, typename...Args>             \
struct has_method_##name<T, Return(Args...)> : has_method_ ##name## _helper<T, Return(Args...)>::type {};
