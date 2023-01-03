#pragma once

#include "binary-tree.hpp"

namespace binary_trees {

template <typename NodeRef>
struct dynamic_binary_tree : binary_tree<NodeRef> {
    using node_ref_type = NodeRef;

    virtual node_ref_type &left_ref(node_ref_type node) = 0;

    virtual node_ref_type &right_ref(node_ref_type node) = 0;

    virtual node_ref_type &parent_ref(node_ref_type node) = 0;

    void right_rotate(node_ref_type x) {
        node_ref_type y = parent(x);
        push_down(y);
        push_down(x);
        left_ref(y) = right(x);
        right_ref(x) = y;
        if (!null(left(y))) {
            parent_ref(left(y)) = y;
        }
        parent_ref(x) = parent(y);
        parent(y) = x;
        if (!null(parent(x))) {
            if (left(parent(x)) == y) {
                left_ref(parent(x)) = x;
            } else {
                right_ref(parent(x)) = x;
            }
        }
        pull_up(y);
        pull_up(x);
    }

    void left_rotate(node_ref_type x) {
        node_ref_type y = parent(x);
        push_down(y);
        push_down(x);
        right_ref(y) = left(x);
        left_ref(x) = y;
        if (!null(right(y))) {
            parent_ref(right(y)) = y;
        }
        parent_ref(x) = parent(y);
        parent_ref(y) = x;
        if (!null(parent(x))) {
            if (left(parent(x)) == y) {
                left_ref(parent(x)) = x;
            } else {
                right_ref(parent(x)) = x;
            }
        }
        pull_up(y);
        pull_up(x);
    }
};

}
