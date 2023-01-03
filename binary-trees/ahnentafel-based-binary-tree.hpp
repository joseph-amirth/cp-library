#pragma once

#include <vector>
#include "binary-tree.hpp"

namespace binary_trees {

struct ahnentafel_based_binary_tree : binary_tree<int> {
    virtual bool null(int node) const {
        return node == 0;
    }

    virtual int left(int node) const {
        return node << 1;
    }

    virtual int right(int node) const {
        return (node << 1) | 1;
    }

    virtual int parent(int node) const {
        return node >> 1;
    }
};

}
