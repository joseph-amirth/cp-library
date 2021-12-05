#pragma once

template<typename treap_node>
struct basic_treap {
    treap_node *parent;
    treap_node *left;
    treap_node *right;

    virtual void pull_up() {}

    virtual void push_down() {}


};