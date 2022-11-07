#pragma once

#include <vector>
#include <functional>
#include "../algebra/type-traits.hpp"

namespace data_structures {

template <typename Groupoid, typename = void>
struct fenwick_tree {
    using groupoid_type = Groupoid;
    using value_type = typename groupoid_type::value_type;

    int n;
    std::vector<value_type> bit;

    fenwick_tree() : n() {}

    fenwick_tree(int n) : n(n), bit(n + 1, groupoid_type::e()) {}

    template <typename Iterator>
    fenwick_tree(Iterator first, Iterator last) : fenwick_tree(std::distance(first, last)) {
        for (int i = 0; first != last; i++, first++) {
            apply(i, *first);
        }
    }

    void apply(int i, const value_type &val) {
        for (++i; i <= n; i += i & -i) {
            bit[i] = groupoid_type::op(bit[i], val);
        }
    }

    value_type prefix_query(int i) const {
        value_type ans = groupoid_type::e();
        for (++i; i; i -= i & -i) {
            ans = groupoid_type::op(ans, bit[i]);
        }
        return ans;
    }

    template <typename T = groupoid_type>
    std::enable_if_t<algebra::is_group_v<T>, value_type> range_query(int l, int r) const {
        if (l == 0) {
            return prefix_query(r);
        } else {
            return groupoid_type::op(prefix_query(r), groupoid_type::inv(prefix_query(l - 1)));
        }
    }
};

}
