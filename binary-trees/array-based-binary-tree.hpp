#pragma once

#include <vector>
#include "binary-tree.hpp"

namespace binary_trees {

struct array_based_binary_tree_node;

template <typename Node>
struct array_based_binary_tree : binary_tree<int> {
    static_assert(std::is_base_of_v<array_based_binary_tree_node, Node>,
                  "Node must inherit from array_based_binary_tree_node");

    using node_type = Node;
    using container_type = std::vector<node_type>;

    container_type nodes;

    array_based_binary_tree() : nodes(1) {}

    virtual bool null(const node_ref_type node) const {
        return !(0 <= node && node < std::size(nodes));
    }

    virtual node_ref_type left(const node_ref_type node) const {
        return nodes[node].left;
    }

    virtual node_ref_type right(const node_ref_type node) const {
        return nodes[node].right;
    }

    virtual node_ref_type parent(const node_ref_type node) const {
        throw std::logic_error("Parent function is not implemented");
    }
};

struct array_based_binary_tree_node {
    int left = 0;
    int right = 0;

    array_based_binary_tree_node() {}

    virtual void pull_up() {}

    virtual void push_down() {}
};

}
