#pragma once

#include <vector>
#include <functional>
#include <cassert>

namespace data_structures {

template <typename Groupoid>
struct disjoint_sparse_table {
    using groupoid_type = Groupoid;
    using value_type = typename groupoid_type::value_type;

    int n;
    std::vector<std::vector<value_type>> table;

    disjoint_sparse_table() : n() {}

    template <typename ArrayLike>
    disjoint_sparse_table(ArrayLike &&v) : n(std::size(v)) {
        assert(!std::empty(v));
        int lg = 32 - __builtin_clz(n);
        table.assign(lg, std::vector<value_type>(std::begin(v), std::end(v)));
        for (int k = 1; k < lg; k++) {
            for (int j = 1 << k; j < n; j += 2 << k) {
                for (int i = j - 2; i >= j - (1 << k); i--) {
                    table[k][i] = groupoid_type::op(v[i], table[k][i + 1]);
                }
                for (int i = j + 1; i < std::min(n, j + (1 << k)); i++) {
                    table[k][i] = groupoid_type::op(table[k][i - 1], v[i]);
                }
            }
        }
    }

    value_type range_query(int l, int r) {
        assert(0 <= l && l <= r && r < n);
        if (l != r) {
            int j = 32 - __builtin_clz(l ^ r) - 1;
            return groupoid_type::op(table[j][l], table[j][r]);
        } else {
            return table[0][l];
        }
    }
};

}
