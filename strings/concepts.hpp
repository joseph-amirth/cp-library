#pragma once

#include <ranges>

namespace strings {

template <typename T>
concept String = std::ranges::random_access_range<T>;

template <typename T>
concept CharSet = requires(T t, typename T::char_type ch, int i) {
    { t.size() } -> std::same_as<int>;
    { t.index_of(ch) } -> std::same_as<int>;
    { t.char_at(i) } -> std::same_as<typename T::char_type>;
    { t.contains(ch) } -> std::same_as<bool>;
};

} // namespace strings
