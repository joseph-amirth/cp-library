#pragma once

#include <vector>
#include <functional>
#include <cassert>

namespace data_structures {

template <typename Semigroup>
struct sparse_table {
    using semigroup_type = Semigroup;
    using value_type = typename semigroup_type::value_type;

    int n;
    std::vector<std::vector<value_type>> table;

    sparse_table() : n() {}

    template <typename Iterator>
    sparse_table(Iterator first, Iterator last) : n(std::distance(first, last)) {
        assert(first != last);
        int lg = 32 - __builtin_clz(n);
        table.resize(lg), table[0] = std::vector<value_type>(first, last);
        for (int j = 1; j < table.size(); j++) {
            table[j].resize(n - (1 << j) + 1);
            for (int i = 0; i + (1 << j) <= n; i++) {
                table[j][i] = semigroup_type::op(table[j - 1][i], table[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    value_type range_query(int l, int r) {
        assert(0 <= l && l <= r && r < n);
        int j = 32 - __builtin_clz(r - l + 1) - 1;
        return semigroup_type::op(table[j][l], table[j][r + 1 - (1 << j)]);
    }
};

}
