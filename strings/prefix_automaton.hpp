#pragma once

#include "prefix_function.hpp"
#include "strings/concepts.hpp"
#include <array>

namespace strings {

template <typename CharSet>
std::vector<std::vector<int>> prefix_automaton(const String auto &s) {
    constexpr auto K = CharSet::size();

    int n = std::size(s);
    std::vector<int> pi = prefix_function(s);
    std::vector<std::vector<int>> automaton(n, std::array<int, K>());
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < K; j++) {
            if (i == 0 || j == CharSet::index_of(s[i])) {
                automaton[i][j] = i;
            } else {
                automaton[i][j] = automaton[pi[i - 1]][j];
            }
        }
    }
    return automaton;
}

} // namespace strings
