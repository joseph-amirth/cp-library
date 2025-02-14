#pragma once

#include "../range_query/splay_tree/implicit.hpp"
#include <vector>

namespace tree {

template <typename Node>
struct link_cut_tree {

    std::vector<Node *> nodes;

    link_cut_tree(int n) : nodes(n, nullptr) {
        for (int i = 0; i < n; i++) {
            nodes[i] = new Node(i);
        }
    }

    int new_vertex() {
        int id = nodes.size();
        nodes.emplace_back(new Node(id));
        return id;
    }

    void access(int u) {
        Node::splay_with_push_down(nodes[u]);
        if (nodes[u]->right != nullptr) {
            nodes[u]->right->parent = nullptr;
            nodes[u]->right->path_parent = nodes[u];
            nodes[u]->right = nullptr;
            nodes[u]->pull_up();
        }
        while (nodes[u]->path_parent != nullptr) {
            int v = nodes[u]->path_parent->vertex;
            Node::splay_with_push_down(nodes[v]);
            if (nodes[v]->right != nullptr) {
                nodes[v]->right->parent = nullptr;
                nodes[v]->right->path_parent = nodes[v];
            }
            nodes[v]->right = nodes[u];
            nodes[u]->parent = nodes[v];
            Node::splay_with_push_down(nodes[u]);
        }
    }

    void make_root(int u) {
        access(u);
        nodes[u]->flip();
    }

    int find(int u) {
        access(u);
        return Node::find_first(nodes[u])->vertex;
    }

    void link(int u, int v) {
        access(u), make_root(v);
        nodes[u]->right = nodes[v];
        nodes[v]->parent = nodes[u];
        nodes[u]->pull_up();
    }

    void cut(int u, int v) {
        make_root(u), access(v);
        nodes[u]->parent = nullptr;
        nodes[v]->left = nullptr;
        nodes[v]->pull_up();
    }

    Node *operator[](int i) {
        return nodes[i];
    }

    ~link_cut_tree() {
        for (Node *node : nodes) {
            delete node;
        }
    }
};

template <typename Node>
struct basic_lct_node : range_query::implicit_splay_tree<Node> {
    using range_query::implicit_splay_tree<Node>::left;
    using range_query::implicit_splay_tree<Node>::right;
    using range_query::implicit_splay_tree<Node>::size;
    using range_query::implicit_splay_tree<Node>::parent;

    int vertex;

    Node *path_parent;
    bool flipped;

    basic_lct_node(int u) : vertex(u) {
        size = 1;
        parent = left = right = nullptr;
        path_parent = nullptr;
        flipped = false;
    }

    void flip() {
        flipped ^= true;
        std::swap(left, right);
    }

    virtual void push_down() {
        if (left != nullptr) {
            if (flipped) {
                left->flip();
            }
            left->path_parent = path_parent;
        }
        if (right != nullptr) {
            if (flipped) {
                right->flip();
            }
            right->path_parent = path_parent;
        }
        flipped = false;
    }
};

} // namespace tree
