#pragma once

#include <vector>

namespace strings {

template<typename String>
std::vector<int> prefix_function(const String &s) {
    int n = std::size(s);
    std::vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            pi[i] = j + 1;
        }
    }
    return pi;
}

}
