#pragma once

#include <vector>
#include <functional>
#include "../algebra/type-traits.hpp"

namespace data_structures {

template <typename Monoid, typename = void>
struct fenwick_tree {
    using monoid_type = Monoid;
    using value_type = typename monoid_type::value_type;

    int n;
    std::vector<value_type> bit;

    fenwick_tree() : n() {}

    fenwick_tree(int n) : n(n), bit(n + 1, monoid_type::e()) {}

    void apply(int i, const value_type &val) {
        for (++i; i <= n; i += i & -i) {
            bit[i] = monoid_type::op(bit[i], val);
        }
    }

    value_type query(int i) const {
        value_type ans = monoid_type::e();
        for (++i; i; i -= i & -i) {
            ans = monoid_type::op(ans, bit[i]);
        }
        return ans;
    }

    template <typename T = monoid_type>
    std::enable_if_t<algebra::is_group_v<T>, value_type> range_query(int l, int r) const {
        if (l == 0) {
            return query(r);
        } else {
            return monoid_type::op(query(r), monoid_type::inv(query(l - 1)));
        }
    }
};

}
