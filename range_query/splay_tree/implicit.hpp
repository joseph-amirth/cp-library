#pragma once

#include "basic.hpp"
#include <cassert>
#include <concepts>
#include <iterator>
#include <utility>

namespace range_query {

template <typename Node>
struct implicit_splay_tree : basic_splay_tree<Node> {
    using basic_splay_tree<Node>::splay;

    int size;

    virtual void pull_up() {
        size = 1 + (this->left ? this->left->size : 0) + (this->right ? this->right->size : 0);
    }

    static Node *find_kth(Node *root, int k) {
        assert(root != nullptr && k < root->size);
        Node *temp = root;
        while (temp != nullptr) {
            temp->push_down();
            if (temp->left != nullptr && temp->left->size > k) {
                temp = temp->left;
            } else {
                if (temp->left != nullptr) {
                    k -= temp->left->size;
                }
                if (k == 0) {
                    break;
                } else {
                    k -= 1;
                    temp = temp->right;
                }
            }
        }
        splay(temp);
        return temp;
    }

    static Node *find_first(Node *root) {
        return find_kth(root, 0);
    }

    static Node *find_last(Node *root) {
        assert(root != nullptr);
        return find_kth(root, root->size - 1);
    }

    static Node *find_root(Node *x) {
        while (x != nullptr) {
            x = x->parent;
        }
        return x;
    }

    static int order_of(Node *x) {
        assert(x != nullptr);
        splay(x);
        if (x->left == nullptr) {
            return 0;
        } else {
            return x->left->size;
        }
    }

    static Node *split(Node **root, int k) {
        *root = find_kth(*root, k);
        Node *other = (*root)->right;
        (*root)->right = nullptr;
        if (other != nullptr) {
            other->parent = nullptr;
        }
        (*root)->pull_up();
        return other;
    }

    static Node *join(Node *root, Node *other) {
        if (root == nullptr) {
            return other;
        } else if (other == nullptr) {
            return root;
        } else {
            root = find_last(root);
            root->right = other;
            other->parent = root;
            root->pull_up();
            return root;
        }
    }

    template <typename... Args>
    static Node *insert(Node *root, int k, Args &&...args) {
        assert(k <= (root == nullptr ? 0 : root->size));
        Node *new_root = new Node(std::forward<Args>(args)...);
        if (root == nullptr) {

        } else if (k == 0) {
            new_root->right = root;
            root->parent = new_root;
        } else if (k == root->size) {
            new_root->left = root;
            root->parent = new_root;
        } else {
            Node *other = split(&root, k - 1);
            new_root->left = root;
            root->parent = new_root;
            new_root->right = other;
            other->parent = new_root;
        }
        new_root->pull_up();
        return new_root;
    }

    static Node *erase(Node *root, int k) {
        root = find_kth(root, k);
        Node *temp = root;
        if (temp->left != nullptr) {
            temp->left->parent = nullptr;
        }
        if (temp->right != nullptr) {
            temp->right->parent = nullptr;
        }
        root = join(temp->left, temp->right);
        delete temp;
        return root;
    }

    template <std::invocable<Node *> F>
    static Node *query(Node *root, int l, int r, F &&f) {
        assert(root != nullptr && l <= r && r < root->size);
        Node *prefix = nullptr, *middle = root, *suffix = nullptr;
        if (r + 1 != middle->size) {
            suffix = split(&middle, r);
        }
        if (l != 0) {
            prefix = middle;
            middle = split(&prefix, l - 1);
        }
        f(middle);
        return join(join(prefix, middle), suffix);
    }

    template <std::forward_iterator I>
    static Node *build_tree(I first, I last) {
        if (first == last) {
            return nullptr;
        } else {
            auto mid = std::next(first, std::distance(first, last) / 2);
            Node *root = new Node(*mid);
            root->left = build_tree(first, mid);
            if (root->left != nullptr) {
                root->left->parent = root;
            }
            root->right = build_tree(std::next(mid), last);
            if (root->right != nullptr) {
                root->right->parent = root;
            }
            root->pull_up();
            return root;
        }
    }
};

} // namespace range_query
