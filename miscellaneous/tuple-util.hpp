#pragma once

#include <tuple>

template <typename...Tuples>
constexpr std::size_t min_tuple_size() {
    if constexpr (sizeof...(Tuples) > 0) {
        auto tuple_sizes = {std::tuple_size_v<std::decay_t<Tuples>>...};
        return std::min(tuple_sizes);
    } else {
        return 0;
    }
}

namespace detail {

template <typename Function, typename Tuple, std::size_t...Indices>
constexpr void apply_for_each_impl(Function &&f, Tuple &&t, std::index_sequence<Indices...>) {
    (f(std::get<Indices>(t)), ...);
}

}

template <typename Function, typename Tuple>
constexpr void apply_for_each(Function &&f, Tuple &&t) {
    detail::apply_for_each_impl(std::forward<Tuple>(t), std::forward<Function>(f),
                                std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>());
}

template <std::size_t N, typename Function, typename...Tuples>
constexpr void apply_for(Function &&f, Tuples &&...tuples) {
    f(std::get<N>(tuples)...);
}

namespace detail {

template <typename Function, typename...Tuples, std::size_t...Indices>
constexpr void apply_zip_impl(Function &&f, std::index_sequence<Indices...>, Tuples &&...tuples) {
    (apply_for<Indices>(f, tuples...), ...);
}

}

template <typename Function, typename...Tuples>
constexpr void apply_zip(Function &&f, Tuples &&...tuples) {
    return detail::apply_zip_impl(std::forward<Function>(f),
                                  std::make_index_sequence<min_tuple_size<Tuples...>()>(), tuples...);
}
