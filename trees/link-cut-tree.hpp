#pragma once

#include "../binary-search-trees/splay-tree/implicit-splay-tree.cpp"
#include <vector>

template<typename lct_node>
struct link_cut_tree : private std::vector<lct_node *> {
    using std::vector<lct_node *>::operator[];

    link_cut_tree(int n) : std::vector<lct_node *>(n) {
        for (int i = 0; i < n; i++) {
            (*this)[i] = new lct_node(i);
        }
    }

    int new_vertex() {
        int id = (int) this->size();
        this->emplace_back(new lct_node(id));
        return id;
    }

    void access(int u) {
        lct_node::splay_with_push_down((*this)[u]);
        if ((*this)[u]->right != nullptr) {
            (*this)[u]->right->parent = nullptr;
            (*this)[u]->right->path_parent = (*this)[u];
            (*this)[u]->right = nullptr;
            (*this)[u]->pull_up();
        }
        while ((*this)[u]->path_parent != nullptr) {
            int v = (*this)[u]->path_parent->vertex;
            lct_node::splay_with_push_down((*this)[v]);
            if ((*this)[v]->right != nullptr) {
                (*this)[v]->right->parent = nullptr;
                (*this)[v]->right->path_parent = (*this)[v];
            }
            (*this)[v]->right = (*this)[u];
            (*this)[u]->parent = (*this)[v];
            lct_node::splay_with_push_down((*this)[u]);
        }
    }

    void make_root(int u) {
        access(u);
        (*this)[u]->flip();
    }

    int find(int u) {
        access(u);
        return lct_node::find_first((*this)[u])->vertex;
    }

    void link(int u, int v) {
        access(u), make_root(v);
        (*this)[u]->right = (*this)[v];
        (*this)[v]->parent = (*this)[u];
        (*this)[u]->pull_up();
    }

    void cut(int u, int v) {
        make_root(u), access(v);
        (*this)[u]->parent = nullptr;
        (*this)[v]->left = nullptr;
        (*this)[v]->pull_up();
    }

    ~link_cut_tree() {
        for (lct_node *node: (*this)) {
            delete node;
        }
    }
};

template<typename lct_node>
struct basic_lct_node : implicit_splay_tree<lct_node> {
    using implicit_splay_tree<lct_node>::left;
    using implicit_splay_tree<lct_node>::right;
    using implicit_splay_tree<lct_node>::size;
    using implicit_splay_tree<lct_node>::parent;

    int vertex;

    lct_node *path_parent;
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