#pragma once

#include <vector>
#include <functional>
#include "../algebra/type-traits.hpp"

namespace data_structures {

template <typename Monoid>
struct fenwick_tree {
    using groupoid = Monoid;
    using value_type = typename groupoid::value_type;

    int n;
    std::vector<value_type> ft;
    groupoid monoid;

    fenwick_tree(int n = 0, groupoid monoid = groupoid()) : n(n), ft(n + 1, groupoid::e()), monoid(monoid) {}

    template <typename Iterator>
    fenwick_tree(Iterator first, Iterator last, groupoid monoid = groupoid()) : fenwick_tree(std::distance(first, last), monoid) {
        for (int i = 1; first != last; i++, first++) {
            int j = i + (i & -i);
            ft[i] = groupoid::op(ft[i], *first);
            if (j <= n) {
                ft[j] = groupoid::op(ft[j], ft[i]);
            }
        }
    }

    void apply(int i, const value_type &val) {
        for (++i; i <= n; i += i & -i) {
            ft[i] = groupoid::op(ft[i], val);
        }
    }

    value_type prefix_query(int i) const {
        value_type ans = groupoid::e();
        for (++i; i; i -= i & -i) {
            ans = groupoid::op(ans, ft[i]);
        }
        return ans;
    }

    template <typename T = groupoid>
    std::enable_if_t<algebra::is_group_v<T>, value_type> range_query(int l, int r) const {
        if (l == 0) {
            return prefix_query(r);
        } else {
            return groupoid::op(prefix_query(r), groupoid::inv(prefix_query(l - 1)));
        }
    }
};

}
