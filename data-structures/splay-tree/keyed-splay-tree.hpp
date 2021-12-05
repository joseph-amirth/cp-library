#pragma once

#include "basic-splay-tree.hpp"
#include <functional>

template<typename splay_node, typename key_t, typename compare_t = std::less<>>
struct keyed_splay_tree : basic_splay_tree<splay_node> {
    using basic_splay_tree<splay_node>::splay;

    static constexpr auto compare = compare_t();

    key_t key;

    static splay_node *find(splay_node *root, const key_t &key) {
        if (root == nullptr) {
            return nullptr;
        }
        splay_node *prev = nullptr, *temp = root;
        while (temp != nullptr) {
            prev = temp;
            if (key == temp->key) {
                splay(temp);
                return temp;
            } else if (compare(key, temp->key)) {
                temp = temp->left;
            } else {
                temp = temp->right;
            }
        }
        splay(prev);
        return prev;
    }

    static splay_node *lower_bound(splay_node *root, const key_t &key) {
        if (root == nullptr) {
            return nullptr;
        }
        splay_node *prev = nullptr, *temp = root;
        splay_node *last_greater = nullptr;
        while (temp != nullptr) {
            prev = temp;
            if (key == temp->key) {
                splay(temp);
                return temp;
            } else if (compare(key, temp->key)) {
                last_greater = temp;
                temp = temp->left;
            } else {
                temp = temp->right;
            }
        }
        splay(prev);
        if (last_greater != nullptr) {
            splay(last_greater);
            return last_greater;
        } else {
            return prev;
        }
    }

    static splay_node *upper_bound(splay_node *root, const key_t &key) {
        if (root == nullptr) {
            return nullptr;
        }
        splay_node *prev = nullptr, *temp = root;
        splay_node *last_greater = nullptr;
        while (temp != nullptr) {
            prev = temp;
            if (compare(key, temp->key)) {
                last_greater = temp;
                temp = temp->left;
            } else {
                temp = temp->right;
            }
        }
        splay(prev);
        if (last_greater != nullptr) {
            splay(last_greater);
            return last_greater;
        } else {
            return prev;
        }
    }

    static splay_node *find_minimum(splay_node *root) {
        if (root == nullptr) {
            return nullptr;
        }
        splay_node *temp = root;
        while (temp->left != nullptr) {
            temp = temp->left;
        }
        splay(temp);
        return temp;
    }

    static splay_node *find_maximum(splay_node *root) {
        if (root == nullptr) {
            return nullptr;
        }
        splay_node *temp = root;
        while (temp->right != nullptr) {
            temp = temp->right;
        }
        splay(temp);
        return temp;
    }

    static splay_node *split(splay_node **root, const key_t &key) {
        if (*root == nullptr) {
            return nullptr;
        } else {
            splay_node *prev = nullptr, *temp = *root;
            splay_node *split_node = nullptr;
            while (temp != nullptr) {
                prev = temp;
                if (key == temp->key) {
                    split_node = temp;
                    break;
                } else if (compare(key, temp->key)) {
                    temp = temp->left;
                } else {
                    split_node = temp;
                    temp = temp->right;
                }
            }
            splay(prev);
            if (split_node == nullptr) {
                split_node = prev;
            }
            splay(split_node);
            splay_node *other = nullptr;
            if (split_node->right != nullptr) {
                other = split_node->right;
                split_node->right = nullptr;
                other->parent = nullptr;
            }
            *root = split_node;
            return other;
        }
    }

    static splay_node *join(splay_node *root, splay_node *other) {
        if (root == nullptr) {
            return other;
        } else if (other == nullptr) {
            return root;
        } else {
            root = find_maximum(root);
            root->right = other;
            other->parent = root;
            root->pull_up();
            return root;
        }
    }

    template<typename...constructor_args>
    static splay_node *insert(splay_node *root, constructor_args...args) {
        if (root == nullptr) {
            return new splay_node(args...);
        } else {
            splay_node *new_root = new splay_node(args...);
            splay_node *prev = nullptr, *temp = root;
            while (temp != nullptr) {
                prev = temp;
                if (temp->key == new_root->key) {
                    splay(temp);
                    delete new_root;
                    return temp;
                } else if (compare(new_root->key, temp->key)) {
                    temp = temp->left;
                } else {
                    temp = temp->right;
                }
            }
            if (compare(new_root->key, prev->key)) {
                prev->left = new_root;
            } else {
                prev->right = new_root;
            }
            new_root->parent = prev;
            splay(new_root);
            return new_root;
        }
    }

    static splay_node *erase(splay_node *root, const key_t &key) {
        if (root = find(root, key), root != nullptr && root->key == key) {
            if (root->left != nullptr) {
                root->left->parent = nullptr;
            }
            if (root->right != nullptr) {
                root->right->parent = nullptr;
            }
            splay_node *new_root = join(root->left, root->right);
            delete root;
            return new_root;
        } else {
            return root;
        }
    }
};