#pragma once

#include <vector>
#include <functional>
#include <cassert>

namespace data_structures {

template <typename Semigroup>
struct disjoint_sparse_table {
    using groupoid = Semigroup;
    using value_type = typename groupoid::value_type;

    int n;
    std::vector<std::vector<value_type>> table;
    groupoid semigroup;

    disjoint_sparse_table() {}

    template <typename Iterator>
    disjoint_sparse_table(Iterator first, Iterator last, groupoid semigroup = groupoid()) : n(std::distance(first, last)), semigroup(semigroup) {
        assert(n > 0);
        int lg = 32 - __builtin_clz(n);
        table.assign(lg, std::vector<value_type>(first, last));
        for (int k = 1; k < lg; k++) {
            for (int j = 1 << k; j < n; j += 2 << k) {
                for (int i = j - 2; i >= j - (1 << k); i--) {
                    table[k][i] = semigroup.op(table[0][i], table[k][i + 1]);
                }
                for (int i = j + 1; i < std::min(n, j + (1 << k)); i++) {
                    table[k][i] = semigroup.op(table[k][i - 1], table[0][i]);
                }
            }
        }
    }

    value_type range_query(int l, int r) {
        assert(0 <= l && l <= r && r < n);
        if (l != r) {
            int j = 32 - __builtin_clz(l ^ r) - 1;
            return semigroup.op(table[j][l], table[j][r]);
        } else {
            return table[0][l];
        }
    }
};

}
