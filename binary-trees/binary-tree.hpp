#pragma once

#include <type_traits>
#include <stdexcept>

namespace binary_trees {

template <typename NodeRef>
struct binary_tree {
    using node_ref_type = NodeRef;

    virtual bool null(const node_ref_type node) const = 0;

    virtual node_ref_type left(const node_ref_type node) const = 0;

    virtual node_ref_type right(const node_ref_type node) const = 0;

    virtual node_ref_type parent(const node_ref_type node) const = 0;

    virtual bool leaf(const node_ref_type node) const {
        return !null(node) && null(left(node)) && null(right(node));
    }

    virtual void pull_up(const node_ref_type node) {}

    virtual void push_down(const node_ref_type node) {}

    template <typename F>
    void visit_preorder(node_ref_type node, F &&f) {
        if (!null(node)) {
            push_down(node);
            f(node);
            visit_preorder(left(node), f);
            visit_preorder(right(node), f);
        }
    }

    template <typename F>
    void visit_inorder(node_ref_type node, F &&f) {
        if (!null(node)) {
            push_down(node);
            visit_inorder(left(node), f);
            f(node);
            visit_inorder(right(node), f);
        }
    }

    template <typename F>
    void visit_postorder(node_ref_type node, F &&f) {
        if (!null(node)) {
            push_down(node);
            visit_postorder(left(node), f);
            visit_postorder(right(node), f);
            f(node);
        }
    }

    template <typename F>
    node_ref_type stab(node_ref_type node, F &&f) {
        while (!null(node)) {
            push_down(node);
            auto decision = f(node);
            if (decision < 0) {
                node = left(node);
            } else if (decision > 0) {
                node = right(node);
            } else {
                break;
            }
        }
        return node;
    }

    template <typename F>
    node_ref_type stab_until_leaf(node_ref_type node, F &&f) {
        while (!leaf(node)) {
            push_down(node);
            auto decision = f(node);
            if (decision < 0) {
                node = left(node);
            } else {
                node = right(node);
            }
        }
        return node;
    }

    node_ref_type leftmost_leaf(node_ref_type node) {
        return stab_until_leaf(node, [](...) -> int {
            return -1;
        });
    }

    node_ref_type rightmost_leaf(node_ref_type node) {
        return stab_until_leaf(node, [](...) -> int {
            return 1;
        });
    }
};

}
