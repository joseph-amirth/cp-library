#pragma once

#include <vector>

template<typename T>
auto inversions(std::vector<T> v) {
    int n = (int) v.size();

    std::vector<T> temp(n);
    auto merge = [&](int l, int m, int r) -> long long {
        long long inversions = 0;
        for (int i = l, j = m; i != m || j != r;) {
            if (j == r || (i < m && v[i] <= v[j])) {
                temp[i - l + j - m] = v[i];
                i += 1;
            } else {
                temp[i - l + j - m] = v[j];
                inversions += m - i;
                j += 1;
            }
        }
        std::copy(temp.begin(), temp.begin() + r - l, v.begin() + l);
        return inversions;
    };

    auto merge_sort = [&](int l, int r, auto &&self) -> long long {
        if (r - l <= 1) {
            return 0;
        } else {
            int m = (l + r) >> 1;
            long long inversions = 0;
            inversions += self(l, m, self);
            inversions += self(m, r, self);
            inversions += merge(l, m, r);
            return inversions;
        }
    };

    long long inversions = merge_sort(0, n, merge_sort);
    return std::make_pair(inversions, v);
}