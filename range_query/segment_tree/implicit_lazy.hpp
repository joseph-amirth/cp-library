#pragma once

#include "algebra/concepts.hpp"
#include "range_query/concepts.hpp"
#include "range_query/segment_tree/implicit.hpp"

namespace range_query {

template <algebra::Monoid M, Initializer<typename M::value_type> I, LazyAction A>
struct lazy_implicit_segment_tree : public basic_implicit_segment_tree<M, I, true> {
    using value_type = typename M::value_type;
    using action_type = typename A::value_type;

    using typename basic_implicit_segment_tree<M, I, true>::node;
    using basic_implicit_segment_tree<M, I, true>::monoid;
    using basic_implicit_segment_tree<M, I, true>::n;
    using basic_implicit_segment_tree<M, I, true>::nodes;
    using basic_implicit_segment_tree<M, I, true>::grow;

    A lazy_action;
    std::vector<action_type> actions;

    template <typename... Args>
    lazy_implicit_segment_tree(Args &&...args) : basic_implicit_segment_tree<M, I, true>(std::forward<Args>(args)...),
                                                 actions(nodes.size(), lazy_action.id()) {}

    virtual int new_node(node node) {
        actions.push_back(lazy_action.id());
        return basic_implicit_segment_tree<M, I, true>::new_node(node);
    }

    virtual void push_down(int i, int l, int r) {
        if (actions[i] == lazy_action.id()) {
            return;
        }
        int m = l + (r - l) / 2;
        auto [left_action, right_action] = lazy_action.split(actions[i], (m - l + 1));
        actions[i] = lazy_action.id();
        int left = nodes[i].left, right = nodes[i].right;
        nodes[left].val = lazy_action.apply(left_action, nodes[left].val, m - l + 1);
        actions[left] = lazy_action.op(left_action, actions[left]);
        nodes[right].val = lazy_action.apply(right_action, nodes[right].val, r - m);
        actions[right] = lazy_action.op(right_action, actions[right]);
    }

    template <typename F>
    void update_range(int ql, int qr, F &&f) {
        assert(0 <= ql && ql <= qr && qr < n);
        auto driver = [&](auto &&self, int i, int l, int r) -> void {
            if (r < ql || qr < l) {
                return;
            } else if (ql <= l && r <= qr) {
                f(i, l, r);
            } else {
                if (nodes[i].left == -1) {
                    grow(i, l, r);
                }
                push_down(i, l, r);
                int m = (l + r) / 2;
                self(self, nodes[i].left, l, m);
                self(self, nodes[i].right, m + 1, r);
                nodes[i].val = monoid.op(nodes[nodes[i].left].val, nodes[nodes[i].right].val);
            }
        };
        driver(driver, 0, 0, n - 1);
    }

    void range_update(int ql, int qr, action_type action) {
        update_range(ql, qr, [&action, this](int i, int l, int r) {
            int size = r - l + 1;
            auto [left_action, right_action] = lazy_action.split(action, size);
            nodes[i].val = lazy_action.apply(action, nodes[i].val, size);
            actions[i] = lazy_action.op(left_action, actions[i]);
            action = right_action;
        });
    }
};

} // namespace range_query
