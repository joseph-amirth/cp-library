#pragma once

#include "basic.hpp"
#include <concepts>
#include <functional>
#include <utility>

namespace range_query {

template <typename Node, typename Key, std::strict_weak_order<Key, Key> Compare = std::less<>>
struct keyed_splay_tree : basic_splay_tree<Node> {
    using basic_splay_tree<Node>::splay;

    static constexpr auto compare = Compare();

    Key key;

    static Node *find(Node *root, const Key &key) {
        if (root == nullptr) {
            return nullptr;
        }
        Node *prev = nullptr, *temp = root;
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

    static Node *lower_bound(Node *root, const Key &key) {
        if (root == nullptr) {
            return nullptr;
        }
        Node *prev = nullptr, *temp = root;
        Node *last_greater = nullptr;
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

    static Node *upper_bound(Node *root, const Key &key) {
        if (root == nullptr) {
            return nullptr;
        }
        Node *prev = nullptr, *temp = root;
        Node *last_greater = nullptr;
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

    static Node *find_minimum(Node *root) {
        if (root == nullptr) {
            return nullptr;
        }
        Node *temp = root;
        while (temp->left != nullptr) {
            temp = temp->left;
        }
        splay(temp);
        return temp;
    }

    static Node *find_maximum(Node *root) {
        if (root == nullptr) {
            return nullptr;
        }
        Node *temp = root;
        while (temp->right != nullptr) {
            temp = temp->right;
        }
        splay(temp);
        return temp;
    }

    static Node *split(Node **root, const Key &key) {
        if (*root == nullptr) {
            return nullptr;
        } else {
            Node *prev = nullptr, *temp = *root;
            Node *split_node = nullptr;
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
            Node *other = nullptr;
            if (split_node->right != nullptr) {
                other = split_node->right;
                split_node->right = nullptr;
                other->parent = nullptr;
            }
            *root = split_node;
            return other;
        }
    }

    static Node *join(Node *root, Node *other) {
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

    template <typename... Args>
    static Node *insert(Node *root, Args &&...args) {
        if (root == nullptr) {
            return new Node(std::forward<Args>(args)...);
        } else {
            Node *new_root = new Node(args...);
            Node *prev = nullptr, *temp = root;
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

    static Node *erase(Node *root, const Key &key) {
        if (root = find(root, key), root != nullptr && root->key == key) {
            if (root->left != nullptr) {
                root->left->parent = nullptr;
            }
            if (root->right != nullptr) {
                root->right->parent = nullptr;
            }
            Node *new_root = join(root->left, root->right);
            delete root;
            return new_root;
        } else {
            return root;
        }
    }
};

} // namespace range_query
