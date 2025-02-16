#pragma once

#include <ranges>

namespace strings {

template <typename T>
concept String = std::ranges::random_access_range<T>;

}
