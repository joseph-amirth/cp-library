#pragma once

#include <concepts>
#include <ranges>

namespace number_theory {

template <typename F, typename T>
concept Factorizer = std::integral<T> && requires(F &&f, T x) {
    { f(x) } -> std::ranges::range;
};

} // namespace number_theory
