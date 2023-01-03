#pragma once

namespace binary_trees {

template <typename Node>
struct subtree_size_invariant : virtual Node {
    int size;

    virtual void pull_up() {}
};

}