#pragma once

#include "../pointer-based-binary-tree.hpp"

namespace binary_trees {

template <typename Node>
struct basic_splay_tree : pointer_based_binary_tree<Node> {
    using node_ref_type = Node *;

    void splay(node_ref_type x) {
        if (x->parent == nullptr) {
            x->push_down();
            return;
        }
        node_ref_type y, z;
        while (y = x->parent, y != nullptr) {
            if (z = y->parent, z == nullptr) {
                y->push_down();
                x->push_down();
                if (y->left == x) {
                    right_rotate(x);
                } else {
                    left_rotate(x);
                }
                y->pull_up();
                x->pull_up();
            } else {
                z->push_down();
                y->push_down();
                x->push_down();
                if (z->left == y && y->left == x) {
                    right_rotate(y);
                    right_rotate(x);
                } else if (z->right == y && y->right == x) {
                    left_rotate(y);
                    left_rotate(x);
                } else if (z->left == y && y->right == x) {
                    left_rotate(x);
                    right_rotate(x);
                } else {
                    right_rotate(x);
                    left_rotate(x);
                }
                z->pull_up();
                y->pull_up();
                x->pull_up();
            }
        }
    }
};

}
