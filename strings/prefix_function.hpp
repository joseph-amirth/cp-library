#pragma once

#include "strings/concepts.hpp"
#include <vector>

namespace strings {

std::vector<int> prefix_function(const String auto &str) {
    int n = std::ranges::size(str);
    auto begin = std::ranges::begin(str);
    std::vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && *(begin + i) != *(begin + j)) {
            j = pi[j - 1];
        }
        if (*(begin + i) == *(begin + j)) {
            pi[i] = j + 1;
        }
    }
    return pi;
}

} // namespace strings
