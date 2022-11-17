#pragma once

#include "prefix-function.hpp"

namespace strings {

template<typename String>
std::vector<int> kmp_pattern_matching(const String &s, const String &t) {
    int n = std::size(s), m = std::size(t);
    std::vector<int> occurrences;
    std::vector<int> pi = prefix_function(t, m);
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && s[i] != t[j]) {
            j = pi[j - 1];
        }
        if (s[i] == t[j]) {
            j += 1;
        }
        if (j == m) {
            occurrences.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }
    return occurrences;
}

}
