#pragma once

#include <vector>

namespace strings {

template <typename String>
std::vector<int> z_function(const String &s) {
    int n = std::size(s);
    std::vector<int> z(n);
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        z[i] = (i > r ? 0 : std::min(r - i + 1, z[i - l]));
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i] += 1;
        }
        if (i + z[i] - 1 > r) {
            l = i, r = i + z[i] - 1;
        }
    }
    return z;
}

}
