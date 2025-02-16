#pragma once

#include "concepts.hpp"
#include "prefix_function.hpp"
#include <ranges>

namespace strings {

std::vector<int> kmp_pattern_matching(const String auto &s, const String auto &t) {
    int n = std::ranges::size(s), m = std::ranges::size(t);
    auto s_begin = std::ranges::begin(s);
    auto t_begin = std::ranges::begin(t);
    std::vector<int> occurrences;
    std::vector<int> pi = prefix_function(t);
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && *(s_begin + i) != *(t_begin + j)) {
            j = pi[j - 1];
        }
        if (*(s_begin + i) == *(t_begin + j)) {
            j += 1;
        }
        if (j == m) {
            occurrences.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }
    return occurrences;
}

} // namespace strings
