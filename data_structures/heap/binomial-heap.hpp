#pragma once

#include <vector>
#include <functional>

template<typename key_t, typename compare_t = std::less<>>
struct binomial_heap {
    static const compare_t compare;

    struct binomial_tree {
        int k;
        key_t key;

        binomial_tree *parent = nullptr;
        binomial_tree *child = nullptr;
        binomial_tree *sibling = nullptr;

        binomial_tree() : k(-1), key() {}

        binomial_tree(key_t key) : k(0), key(key) {}
    };

    static binomial_tree *merge(binomial_tree *x, binomial_tree *y) {
        if (x == nullptr) {
            return y;
        } else if (y == nullptr) {
            return x;
        }
        assert(x->k == y->k);
        if (compare(y->key, x->key)) {
            std::swap(x, y);
        }
        y->sibling = x->child;
        y->parent = x;
        x->child = y;
        x->k += 1;
        return x;
    }

    std::vector<binomial_tree *> trees;

    binomial_heap() {}

    binomial_heap(binomial_tree *tree) {
        while (tree != nullptr) {
            trees.push_back(tree);
            binomial_tree *sibling = tree->sibling;
            tree->parent = tree->sibling = nullptr;
            tree = sibling;
        }
        std::reverse(trees.begin(), trees.end());
    }

    key_t find_minimum() const {
        assert(!trees.empty());
        key_t *minimum = nullptr;
        for (binomial_tree *tree: trees) {
            if (tree != nullptr && (minimum == nullptr || compare(tree->key, *minimum))) {
                minimum = &(tree->key);
            }
        }
        return *minimum;
    }

    void merge(const binomial_heap &y) {
        if (trees.size() < y.trees.size()) {
            trees.resize(y.trees.size());
        }
        binomial_tree *carry = nullptr;
        for (int i = 0; i < (int) trees.size(); i++) {
            if (i < (int) y.trees.size() && y.trees[i] != nullptr) {
                carry = merge(carry, y.trees[i]);
            }
            if (carry != nullptr && carry->k == i) {
                carry = merge(carry, trees[i]);
                trees[i] = nullptr;
                if (carry->k == i) {
                    std::swap(trees[i], carry);
                }
            }
        }
        if (carry != nullptr) {
            trees.push_back(carry);
        }
    }

    key_t extract_minimum() {
        key_t minimum = find_minimum();
        for (int i = 0; i < (int) trees.size(); i++) {
            if (trees[i] != nullptr && minimum == trees[i]->key) {
                binomial_tree *tree = trees[i];
                trees[i] = nullptr;
                merge(binomial_heap(tree->child));
                delete tree;
                break;
            }
        }
        if (trees.back() == nullptr) {
            trees.pop_back();
        }
        return minimum;
    }

    binomial_tree *insert(const key_t &key) {
        binomial_tree *new_node = new binomial_tree(key);
        binomial_tree *carry = new_node;
        for (int i = 0; i < (int) trees.size(); i++) {
            if (trees[i] == nullptr) {
                std::swap(trees[i], carry);
                break;
            }
            carry = merge(carry, trees[i]);
            trees[i] = nullptr;
        }
        if (carry != nullptr) {
            trees.push_back(carry);
        }
        return new_node;
    }

    template<typename...constructor_args>
    binomial_tree *emplace(constructor_args...args) {
        return insert(key_t(args...));
    }

    void swap_with_parent(binomial_tree *x) {
        binomial_tree *y = x->parent;
        x->parent = y->parent, y->parent = x;
        std::swap(x->k, y->k);
        if (x == y->child) {
            y->child = x->sibling;
            x->sibling = y->sibling;
            y->sibling = x->child;
            x->child = y;
        } else {
            for (binomial_tree *subtree = y->child;; subtree = subtree->sibling) {
                subtree->parent = x;
                if (subtree->sibling == x) {
                    binomial_tree *y_child = y->child;
                    subtree->sibling = y;
                    y->child = x->sibling;
                    x->sibling = y->sibling;
                    y->sibling = x->child;
                    x->child = y_child;
                    break;
                }
            }
        }
        if (x->parent != nullptr) {
            binomial_tree *z = x->parent;
            if (z->child == y) {
                z->child = x;
            } else {
                for (binomial_tree *subtree = z->child;; subtree = subtree->sibling) {
                    if (subtree->sibling == y) {
                        subtree->sibling = x;
                        break;
                    }
                }
            }
        }
    }

    void decrease_key(binomial_tree *x, const key_t &key) {
        assert(compare(key, x->key));
        x->key = key;
        while (x->parent != nullptr && compare(x->key, x->parent->key)) {
            swap_with_parent(x);
        }
        if (x->parent == nullptr) {
            trees[x->k] = x;
        }
    }

    void remove(binomial_tree *x) {
        while (x->parent != nullptr) {
            swap_with_parent(x);
        }
        trees[x->k] = nullptr;
        merge(binomial_heap(x->child));
        delete x;
        if (trees.back() == nullptr) {
            trees.pop_back();
        }
    }
};

template<typename key_t, typename compare_t>
const compare_t binomial_heap<key_t, compare_t>::compare = compare_t();