#pragma once

#include "basic-splay-tree.hpp"
#include <iterator>
#include <cassert>

template<typename splay_node>
struct implicit_splay_tree : basic_splay_tree<splay_node> {
    using basic_splay_tree<splay_node>::splay;

    int size;

    virtual void pull_up() {
        size = 1 + (this->left ? this->left->size : 0) + (this->right ? this->right->size : 0);
    }

    static splay_node *find_kth(splay_node *root, int k) {
        assert(root != nullptr && k < root->size);
        splay_node *temp = root;
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

    static splay_node *find_first(splay_node *root) {
        return find_kth(root, 0);
    }

    static splay_node *find_last(splay_node *root) {
        assert(root != nullptr);
        return find_kth(root, root->size - 1);
    }

    static splay_node *find_root(splay_node *x) {
        while (x != nullptr) {
            x = x->parent;
        }
        return x;
    }

    static int order_of(splay_node *x) {
        assert(x != nullptr);
        splay(x);
        if (x->left == nullptr) {
            return 0;
        } else {
            return x->left->size;
        }
    }

    static splay_node *split(splay_node **root, int k) {
        *root = find_kth(*root, k);
        splay_node *other = (*root)->right;
        (*root)->right = nullptr;
        if (other != nullptr) {
            other->parent = nullptr;
        }
        (*root)->pull_up();
        return other;
    }

    static splay_node *join(splay_node *root, splay_node *other) {
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

    template<typename...constructor_args>
    static splay_node *insert(splay_node *root, int k, constructor_args...args) {
        assert(k <= (root == nullptr ? 0 : root->size));
        splay_node *new_root = new splay_node(args...);
        if (root == nullptr) {

        } else if (k == 0) {
            new_root->right = root;
            root->parent = new_root;
        } else if (k == root->size) {
            new_root->left = root;
            root->parent = new_root;
        } else {
            splay_node *other = split(&root, k - 1);
            new_root->left = root;
            root->parent = new_root;
            new_root->right = other;
            other->parent = new_root;
        }
        new_root->pull_up();
        return new_root;
    }

    static splay_node *erase(splay_node *root, int k) {
        root = find_kth(root, k);
        splay_node *temp = root;
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

    template<typename F>
    static splay_node *query(splay_node *root, int l, int r, F &&f) {
        assert(root != nullptr && l <= r && r < root->size);
        splay_node *prefix = nullptr, *middle = root, *suffix = nullptr;
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

    template<typename iterator_t>
    static splay_node *build_tree(iterator_t first, iterator_t last) {
        if (first == last) {
            return nullptr;
        } else {
            auto mid = std::next(first, std::distance(first, last) / 2);
            splay_node *root = new splay_node(*mid);
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