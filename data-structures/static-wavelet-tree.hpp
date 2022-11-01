#pragma once

#include <vector>
#include <cassert>

#include "../miscellaneous/coordinate-compress.hpp"

namespace data_structures {

struct static_wavelet_tree {
    int n;
    coordinate_compress<int> cc;
    std::vector<std::vector<int>> t;

    template <typename Iterator>
    static_wavelet_tree(Iterator first, Iterator last) : n(std::distance(first, last)), cc(first, last) {
        auto build = [&](auto &&self, std::vector<int> &&v, int i, int l, int r) -> void {
            if (!v.empty() && l != r) {
                int m = l + (r - l) / 2;

                int cnt = 0;
                std::vector<int> left, right;
                t[i].reserve(v.size());

                for (int x : v) {
                    if (x <= m) {
                        cnt += 1;
                        left.push_back(x);
                    } else {
                        right.push_back(x);
                    }
                    t[i].push_back(cnt);
                }

                int j = 2 * (m - l + 1);
                self(self, std::move(left), i + 1, l, m);
                self(self, std::move(right), i + j, m + 1, r);
            }
        };
        auto v = std::vector<int>(first, last);
        cc.process();
        cc.compress(v.begin(), v.end());
        t.resize(2 * cc.sigma);
        build(build, std::move(v), 1, 0, cc.sigma - 1);
    }

    int rank(int qr, int value) {
        assert(0 <= qr && qr < n);
        assert(0 <= value && value < cc.sigma);

        auto recurse = [&](auto &&self, int i, int l, int r) -> int {
            if (qr == -1 || l == r || t[i].empty()) {
                return qr + 1;
            } else {
                int m = l + (r - l) / 2;
                int j = 2 * (m - l + 1);
                if (value <= m) {
                    qr = t[i][qr] - 1;
                    return self(self, i + 1, l, m);
                } else {
                    qr = qr - t[i][qr];
                    return self(self, i + j, m + 1, r);
                }
            }
        };
        return recurse(recurse, 1, 0, cc.sigma - 1);
    }

    int rank(int ql, int qr, int value) {
        assert(0 <= ql && ql <= qr && qr < n);
        assert(0 <= value && value < cc.sigma);

        return rank(qr, value) - (ql == 0 ? 0 : rank(ql - 1, value));
    }

    int range_quantile(int ql, int qr, int k) {
        assert(0 <= ql && ql <= qr && qr < n);
        assert(0 <= k && k < qr - ql + 1);

        auto recurse = [&](auto &&self, int i, int l, int r) -> int {
            if (l == r) {
                return cc.values[l];
            } else {
                int m = l + (r - l) / 2;
                int j = 2 * (m - l + 1);
                int c = t[i][qr] - (ql == 0 ? 0 : t[i][ql - 1]);
                if (k + 1 <= c) {
                    ql = (ql == 0 ? 0 : t[i][ql - 1]);
                    qr = t[i][qr] - 1;
                    return self(self, i + 1, l, m);
                } else {
                    ql = (ql == 0 ? 0 : ql - t[i][ql - 1]);
                    qr = qr - t[i][qr];
                    k -= c;
                    return self(self, i + j, m + 1, r);
                }
            }
        };
        return recurse(recurse, 1, 0, cc.sigma - 1);
    }
};

}