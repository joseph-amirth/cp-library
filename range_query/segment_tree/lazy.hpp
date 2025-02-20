#pragma once

#include "algebra/concepts.hpp"
#include "range_query/concepts.hpp"
#include "range_query/segment_tree/basic.hpp"

namespace range_query {

template <algebra::Monoid M, LazyAction A>
struct lazy_segment_tree : public basic_segment_tree<M, true> {
    using value_type = typename M::value_type;
    using action_type = typename A::value_type;

    using basic_segment_tree<M, true>::n;
    using basic_segment_tree<M, true>::t;
    using basic_segment_tree<M, true>::split_range;

    A lazy_action;
    std::vector<action_type> actions;

    template <typename... Args>
    lazy_segment_tree(Args &&...args) : basic_segment_tree<M, true>(std::forward<Args>(args)...) {
        actions.assign(4 * n, lazy_action.id());
    }

    virtual void push_down(int i, int l, int r) {
        if (actions[i] == lazy_action.id()) {
            return;
        }
        int m = l + (r - l) / 2;
        auto [left_action, right_action] = lazy_action.split(actions[i], (m - l + 1));
        actions[i] = lazy_action.id();
        t[i << 1] = lazy_action.apply(left_action, t[i << 1], m - l + 1);
        actions[i << 1] = lazy_action.op(left_action, actions[i << 1]);
        t[i << 1 | 1] = lazy_action.apply(right_action, t[i << 1 | 1], r - m);
        actions[i << 1 | 1] = lazy_action.op(right_action, actions[i << 1 | 1]);
    }

    void range_update(int ql, int qr, action_type action) {
        basic_segment_tree<M, true>::template split_range<true>(ql, qr, [&action, this](int i, int l, int r) {
            int size = r - l + 1;
            auto [left_action, right_action] = lazy_action.split(action, size);
            t[i] = lazy_action.apply(left_action, t[i], size);
            actions[i] = lazy_action.op(left_action, actions[i]);
            action = right_action;
        });
    }
};

} // namespace range_query
