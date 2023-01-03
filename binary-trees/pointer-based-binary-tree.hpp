#pragma once

#include "dynamic-binary-tree.hpp"

namespace binary_trees {

struct pointer_based_binary_tree_node;

template <typename Node>
struct pointer_based_binary_tree : dynamic_binary_tree<Node *> {
    static_assert(std::is_base_of_v<pointer_based_binary_tree_node, Node>,
                  "Node must inherit from pointer_based_binary_tree_node");

    using node_type = Node;
    using node_ref_type = Node *;

    virtual bool null(const node_ref_type node) const {
        return node == nullptr;
    }

    virtual node_ref_type left(const node_ref_type node) const {
        return node->left;
    }

    virtual node_ref_type right(const node_ref_type node) const {
        return node->right;
    }

    virtual node_ref_type parent(const node_ref_type node) const {
        return node->parent;
    }

    virtual node_ref_type &left_ref(node_ref_type node) {
        return node->left;
    }

    virtual node_ref_type &right_ref(node_ref_type node) {
        return node->right;
    }

    virtual node_ref_type &parent_ref(node_ref_type node) {
        return node->parent;
    }

//    void right_rotate(const node_ref_type x) {
//        const node_ref_type y = x->parent;
//        y->push_down();
//        x->push_down();
//        y->left = x->right;
//        x->right = y;
//        if (y->left != nullptr) {
//            y->left->parent = y;
//        }
//        x->parent = y->parent;
//        y->parent = x;
//        if (x->parent != nullptr) {
//            if (x->parent->left == y) {
//                x->parent->left = x;
//            } else {
//                x->parent->right = x;
//            }
//        }
//        y->pull_up();
//        x->pull_up();
//    }

//    void left_rotate(const node_ref_type x) {
//        const node_ref_type y = x->parent;
//        y->push_down();
//        x->push_down();
//        y->right = x->left;
//        x->left = y;
//        if (y->right != nullptr) {
//            y->right->parent = y;
//        }
//        x->parent = y->parent;
//        y->parent = x;
//        if (x->parent != nullptr) {
//            if (x->parent->left == y) {
//                x->parent->left = x;
//            } else {
//                x->parent->right = x;
//            }
//        }
//        y->pull_up();
//        x->pull_up();
//    }

    void delete_tree(const node_ref_type root) {
        if (root != nullptr && root->parent != nullptr) {
            if (root->parent->left == root) {
                root->parent->left = nullptr;
            } else {
                root->parent->right = nullptr;
            }
        }
        visit_postorder(root, [](node_ref_type x) {
            delete x;
        });
    }
};

struct pointer_based_binary_tree_node {
    using node_type = pointer_based_binary_tree_node;

    node_type *left = nullptr;
    node_type *right = nullptr;
    node_type *parent = nullptr;

    pointer_based_binary_tree_node() {}

    virtual void pull_up() {}

    virtual void push_down() {}
};

}
