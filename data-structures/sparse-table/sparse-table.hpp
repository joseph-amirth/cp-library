#pragma once

#include <vector>
#include <functional>
#include <cassert>

namespace data_structures {

template <typename Semigroup>
struct sparse_table {
    using groupoid = Semigroup;
    using value_type = typename groupoid::value_type;

    int n;
    std::vector<std::vector<value_type>> table;
    groupoid semigroup;

    sparse_table() : n() {}

    template <typename Iterator>
    sparse_table(Iterator first, Iterator last, groupoid semigroup = groupoid()) : n(std::distance(first, last)), semigroup(semigroup) {
        assert(n > 0);
        int lg = 32 - __builtin_clz(n);
        table.resize(lg), table[0] = std::vector<value_type>(first, last);
        for (int j = 1; j < table.size(); j++) {
            table[j].resize(n - (1 << j) + 1);
            for (int i = 0; i + (1 << j) <= n; i++) {
                table[j][i] = semigroup.op(table[j - 1][i], table[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    value_type range_query(int l, int r) {
        assert(0 <= l && l <= r && r < n);
        int j = 32 - __builtin_clz(r - l + 1) - 1;
        return semigroup.op(table[j][l], table[j][r + 1 - (1 << j)]);
    }
};

}
