#pragma once

#include "concepts.hpp"
#include <ranges>
#include <vector>

namespace strings {

std::vector<int> z_function(const String auto &str) {
    int n = std::ranges::size(str);
    auto begin = std::ranges::begin(str);
    std::vector<int> z(n);
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        z[i] = (i > r ? 0 : std::min(r - i + 1, z[i - l]));
        while (i + z[i] < n && *(begin + z[i]) == *(begin + i + z[i])) {
            z[i] += 1;
        }
        if (i + z[i] - 1 > r) {
            l = i, r = i + z[i] - 1;
        }
    }
    return z;
}

} // namespace strings
