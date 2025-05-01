#pragma once

#include "algebra/concepts.hpp"

#include <cassert>
#include <vector>

namespace range_query {

template <typename I, typename T>
concept Initializer = requires(I init, int l, int r) {
    { init(l, r) } -> std::same_as<T>;
};

template <algebra::Monoid M, Initializer<typename M::value_type> I, bool lazy = false>
struct basic_implicit_segment_tree {
    using value_type = typename M::value_type;

    struct node {
        value_type val;
        int left, right;

        node(value_type val) : val(val), left(-1), right(-1) {}

        node(value_type val, int left, int right) : val(val), left(left), right(right) {}
    };

    M monoid;
    I initializer;
    int n;
    std::vector<node> nodes;

    virtual int new_node(node node) {
        int id = (int)nodes.size();
        nodes.push_back(node);
        return id;
    }

    virtual void grow(int i, int l, int r) {
        int m = (l + r) / 2;
        nodes[i].left = new_node(node(initializer(l, m)));
        nodes[i].right = new_node(node(initializer(m + 1, r)));
    }

    virtual void push_down(int i, int l, int r) {}

    basic_implicit_segment_tree(M monoid, I initializer, int n) : monoid(monoid), initializer(initializer), n(n) {
        new_node(node(initializer(0, n - 1)));
    }

    basic_implicit_segment_tree(I initializer, int n) : basic_implicit_segment_tree(M(), initializer, n) {}
    basic_implicit_segment_tree(M monoid, int n) : basic_implicit_segment_tree(monoid, I(), n) {}
    basic_implicit_segment_tree(int n) : basic_implicit_segment_tree(M(), I(), n) {}

    template <typename F>
    void visit_point(int qi, F &&f) {
        assert(0 <= qi && qi < n);
        auto driver = [&](auto &&self, int i, int l, int r) -> void {
            if (l == r) {
                f(nodes[i].val);
            } else {
                if (nodes[i].left == -1) {
                    grow(i, l, r);
                }
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                if (qi <= m) {
                    self(self, nodes[i].left, l, m);
                } else {
                    self(self, nodes[i].right, m + 1, r);
                }
            }
        };
        driver(driver, 0, 0, n - 1);
    }

    template <typename F>
    void update_point(int qi, F &&f) {
        assert(0 <= qi && qi < n);
        auto driver = [&](auto &&self, int i, int l, int r) -> void {
            if (l == r) {
                f(nodes[i].val);
            } else {
                if (nodes[i].left == -1) {
                    grow(i, l, r);
                }
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                if (qi <= m) {
                    self(self, nodes[i].left, l, m);
                } else {
                    self(self, nodes[i].right, m + 1, r);
                }
                nodes[i].val = monoid.op(nodes[nodes[i].left].val, nodes[nodes[i].right].val);
            }
        };
        driver(driver, 0, 0, n - 1);
    }

    template <typename F>
    void visit_range(int ql, int qr, F &&f) {
        assert(0 <= ql && ql <= qr && qr < n);
        auto driver = [&](auto &&self, int i, int l, int r) -> void {
            if (r < ql || qr < l) {
                return;
            } else if (ql <= l && r <= qr) {
                f(nodes[i].val);
            } else {
                if (nodes[i].left == -1) {
                    grow(i, l, r);
                }
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                self(self, nodes[i].left, l, m);
                self(self, nodes[i].right, m + 1, r);
            }
        };
        driver(driver, 0, 0, n - 1);
    }

    value_type range_query(int l, int r) {
        value_type ans = monoid.id();
        visit_range(l, r, [&ans, this](value_type &val) {
            ans = monoid.op(ans, val);
        });
        return ans;
    }

    value_type all() {
        return nodes.front().val;
    }
};

template <algebra::Monoid M, Initializer<typename M::value_type> I>
struct implicit_segment_tree : public basic_implicit_segment_tree<M, I, false> {
    using basic_implicit_segment_tree<M, I, false>::basic_implicit_segment_tree;
};

} // namespace range_query
