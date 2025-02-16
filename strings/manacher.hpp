#pragma once

#include "concepts.hpp"
#include <ranges>
#include <vector>

namespace strings {

auto count_palindromes(const String auto &s) {
    int n = std::ranges::size(s);
    auto begin = std::ranges::begin(s);
    std::vector<int> d_odd(n);
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int j = (i > r ? 1 : std::min(d_odd[l + r - i], r - i + 1));
        while (0 <= i - j && i + j < n && *(begin + i - j) == *(begin + i + j)) {
            j += 1;
        }
        d_odd[i] = j--;
        if (i + j > r) {
            l = i - j;
            r = i + j;
        }
    }
    std::vector<int> d_even(n);
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int j = (i > r ? 0 : std::min(d_even[l + r - i + 1], r - i + 1));
        while (0 <= i - j - 1 && i + j < n && *(begin + i - j - 1) == *(begin + i + j)) {
            j += 1;
        }
        d_even[i] = j--;
        if (i + j > r) {
            l = i - j - 1;
            r = i + j;
        }
    }
    return std::make_pair(d_odd, d_even);
}

} // namespace strings
