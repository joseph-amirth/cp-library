#pragma once

#include <concepts>

namespace range_query {

template <typename Node>
struct basic_splay_tree {
    Node *parent;
    Node *left;
    Node *right;

    virtual void pull_up() {}

    virtual void push_down() {}

    virtual ~basic_splay_tree() {}

    static void right_rotate(Node *x, Node *y) {
        y->left = x->right;
        x->right = y;
        if (y->left != nullptr) {
            y->left->parent = y;
        }
        x->parent = y->parent;
        y->parent = x;
        if (x->parent != nullptr) {
            if (x->parent->left == y) {
                x->parent->left = x;
            } else {
                x->parent->right = x;
            }
        }
    }

    static void left_rotate(Node *x, Node *y) {
        y->right = x->left;
        x->left = y;
        if (y->right != nullptr) {
            y->right->parent = y;
        }
        x->parent = y->parent;
        y->parent = x;
        if (x->parent != nullptr) {
            if (x->parent->left == y) {
                x->parent->left = x;
            } else {
                x->parent->right = x;
            }
        }
    }

    static void splay(Node *x) {
        Node *y, *z;
        while (y = x->parent, y != nullptr) {
            if (z = y->parent, z == nullptr) {
                if (y->left == x) {
                    right_rotate(x, y);
                } else {
                    left_rotate(x, y);
                }
                y->pull_up();
                x->pull_up();
            } else {
                if (z->left == y && y->left == x) {
                    right_rotate(y, z);
                    right_rotate(x, y);
                } else if (z->right == y && y->right == x) {
                    left_rotate(y, z);
                    left_rotate(x, y);
                } else if (z->left == y && y->right == x) {
                    left_rotate(x, y);
                    right_rotate(x, z);
                } else {
                    right_rotate(x, y);
                    left_rotate(x, z);
                }
                z->pull_up();
                y->pull_up();
                x->pull_up();
            }
        }
    }

    static void splay_with_push_down(Node *x) {
        if (x->parent == nullptr) {
            x->push_down();
            return;
        }
        Node *y, *z;
        while (y = x->parent, y != nullptr) {
            if (z = y->parent, z == nullptr) {
                y->push_down();
                x->push_down();
                if (y->left == x) {
                    right_rotate(x, y);
                } else {
                    left_rotate(x, y);
                }
                y->pull_up();
                x->pull_up();
            } else {
                z->push_down();
                y->push_down();
                x->push_down();
                if (z->left == y && y->left == x) {
                    right_rotate(y, z);
                    right_rotate(x, y);
                } else if (z->right == y && y->right == x) {
                    left_rotate(y, z);
                    left_rotate(x, y);
                } else if (z->left == y && y->right == x) {
                    left_rotate(x, y);
                    right_rotate(x, z);
                } else {
                    right_rotate(x, y);
                    left_rotate(x, z);
                }
                z->pull_up();
                y->pull_up();
                x->pull_up();
            }
        }
    }

    template <std::invocable<Node *> F>
    static void visit_preorder(Node *root, F &&f) {
        if (root != nullptr) {
            root->push_down();
            f(root);
            visit_preorder(root->left, f);
            visit_preorder(root->right, f);
        }
    }

    template <std::invocable<Node *> F>
    static void visit_inorder(Node *root, F &&f) {
        if (root != nullptr) {
            root->push_down();
            visit_inorder(root->left, f);
            f(root);
            visit_inorder(root->right, f);
        }
    }

    template <std::invocable<Node *> F>
    static void visit_postorder(Node *root, F &&f) {
        if (root != nullptr) {
            root->push_down();
            visit_postorder(root->left, f);
            visit_postorder(root->right, f);
            f(root);
        }
    }

    static void delete_tree(Node *root) {
        if (root != nullptr && root->parent != nullptr) {
            if (root->parent->left == root) {
                root->parent->left = nullptr;
            } else {
                root->parent->right = nullptr;
            }
        }
        visit_postorder(root, [](Node *x) {
            delete x;
        });
    }
};

} // namespace range_query
