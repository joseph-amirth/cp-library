#pragma once

#include <array>

#include "prefix-function.hpp"
#include "../char-set.hpp"

namespace strings {

template <typename CharSet, typename String>
std::vector<std::vector<int>> prefix_automaton(const String &s) {
    using char_set_type = CharSet;

    constexpr auto K = char_set_type::size();

    int n = std::size(s);
    std::vector<int> pi = prefix_function(s);
    std::vector<std::vector<int>> automaton(n, std::array<int, K>());
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < K; j++) {
            if (i == 0 || j == char_set_type::index_of(s[i])) {
                automaton[i][j] = i;
            } else {
                automaton[i][j] = automaton[pi[i - 1]][j];
            }
        }
    }
    return automaton;
}

}
