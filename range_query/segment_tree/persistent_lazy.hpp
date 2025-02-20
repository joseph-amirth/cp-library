#pragma once

#include "algebra/concepts.hpp"
#include "range_query/concepts.hpp"
#include "range_query/segment_tree/persistent.hpp"

namespace range_query {

template <algebra::Monoid M, LazyAction A>
struct lazy_persistent_segment_tree : public basic_persistent_segment_tree<M, true> {
    using value_type = typename M::value_type;
    using action_type = typename A::value_type;

    using typename basic_persistent_segment_tree<M, true>::node;
    using basic_persistent_segment_tree<M, true>::monoid;
    using basic_persistent_segment_tree<M, true>::n;
    using basic_persistent_segment_tree<M, true>::nodes;
    using basic_persistent_segment_tree<M, true>::versions;

    A lazy_action;
    std::vector<action_type> actions;

    template <typename... Args>
    lazy_persistent_segment_tree(Args &&...args) : basic_persistent_segment_tree<M, true>(std::forward<Args>(args)...),
                                                   actions(nodes.size(), lazy_action.id()) {}

    virtual int new_node(node node) {
        actions.push_back(lazy_action.id());
        return basic_persistent_segment_tree<M, true>::new_node(node);
    }

    virtual void push_down(int i, int l, int r) {
        if (actions[i] == lazy_action.id()) {
            return;
        }
        int m = l + (r - l) / 2;
        auto [left_action, right_action] = lazy_action.split(actions[i], (m - l + 1));
        actions[i] = lazy_action.id();
        int left = new_node(nodes[nodes[i].left]);
        int right = new_node(nodes[nodes[i].right]);
        nodes[left].val = lazy_action.apply(left_action, nodes[left].val, m - l + 1);
        actions[left] = lazy_action.op(left_action, actions[nodes[i].left]);
        nodes[right].val = lazy_action.apply(right_action, nodes[right].val, r - m);
        actions[right] = lazy_action.op(right_action, actions[nodes[i].right]);
        nodes[i].left = left, nodes[i].right = right;
    }

    template <typename F>
    int update_range(int version, int ql, int qr, F &&f) {
        assert(0 <= ql && ql <= qr && qr < n);
        auto driver = [&](auto &&self, int i, int l, int r) -> int {
            if (r < ql || qr < l) {
                return i;
            } else if (ql <= l && r <= qr) {
                return f(i, l, r);
            } else {
                push_down(i, l, r);
                int m = (l + r) / 2;
                int left = self(self, nodes[i].left, l, m);
                int right = self(self, nodes[i].right, m + 1, r);
                return new_node(node(monoid.op(nodes[left].val, nodes[right].val), left, right));
            }
        };
        versions.push_back(driver(driver, version, 0, n - 1));
        return versions.back();
    }

    int range_update(int version, int ql, int qr, action_type action) {
        return update_range(version, ql, qr, [&action, this](int i, int l, int r) {
            int size = r - l + 1;
            auto [left_action, right_action] = lazy_action.split(action, size);
            int new_i = new_node(nodes[i]);
            nodes[new_i].val = lazy_action.apply(action, nodes[i].val, size);
            actions[new_i] = lazy_action.op(left_action, actions[i]);
            action = right_action;
            return new_i;
        });
    }
};

} // namespace range_query
