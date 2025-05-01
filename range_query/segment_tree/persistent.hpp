#pragma once

#include "algebra/concepts.hpp"

#include <cassert>
#include <vector>

namespace range_query {

template <algebra::Monoid M, bool lazy = false>
struct basic_persistent_segment_tree {
    using value_type = typename M::value_type;

    struct node {
        value_type val;
        int left, right;

        node(value_type val) : val(val), left(-1), right(-1) {} // leaf

        node(value_type val, int left, int right) : val(val), left(left), right(right) {}
    };

    M monoid;
    int n;
    std::vector<node> nodes;
    std::vector<int> versions;

    virtual int new_node(node node) {
        int id = (int)nodes.size();
        nodes.push_back(node);
        return id;
    }

    virtual void push_down(int i, int l, int r) {}

    template <typename Function>
    int build(Function &&f) {
        auto driver = [&](auto &&self, int l, int r) -> int {
            if (l == r) {
                return new_node(node(f(l)));
            } else {
                int m = (l + r) / 2;
                int left = self(self, l, m);
                int right = self(self, m + 1, r);
                value_type val = monoid.op(nodes[left].val, nodes[right].val);
                return new_node(node(val, left, right));
            }
        };
        return driver(driver, 0, n - 1);
    }

    basic_persistent_segment_tree(M monoid, int n) : monoid(monoid), n(n) {
        versions.push_back(build([&monoid](...) {
            return monoid.id();
        }));
    }

    template <std::forward_iterator I>
    basic_persistent_segment_tree(M monoid, I first, I last) : monoid(monoid), n(std::distance(first, last)) {
        versions.push_back(build([&first](...) {
            return *first++;
        }));
    }

    template <std::forward_iterator I>
    basic_persistent_segment_tree(I first, I last) : basic_persistent_segment_tree(M(), first, last) {}

    template <typename F>
    void visit_point(int version, int qi, F &&f) {
        assert(0 <= qi && qi < n);
        auto driver = [&](auto &&self, int i, int l, int r) -> void {
            if (l == r) {
                f(nodes[i].val);
            } else {
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
        driver(driver, version, 0, n - 1);
    }

    template <typename F>
    int update_point(int version, int qi, F &&f) {
        assert(0 <= qi && qi < n);
        auto driver = [&](auto &&self, int i, int l, int r) -> int {
            int new_i = new_node(node(nodes[i]));
            if (l == r) {
                f(nodes[new_i].val);
            } else {
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                if (qi <= m) {
                    nodes[new_i].left = self(self, nodes[i].left, l, m);
                } else {
                    nodes[new_i].right = self(self, nodes[i].right, m + 1, r);
                }
                nodes[new_i].val = monoid.op(nodes[nodes[new_i].left].val, nodes[nodes[new_i].right].val);
            }
            return new_i;
        };
        versions.push_back(driver(driver, version, 0, n - 1));
        return versions.back();
    }

    template <typename F>
    void visit_range(int version, int ql, int qr, F &&f) {
        assert(0 <= ql && ql <= qr && qr < n);
        auto driver = [&](auto &&self, int i, int l, int r) -> void {
            if (r < ql || qr < l) {
                return;
            } else if (ql <= l && r <= qr) {
                f(nodes[i].val);
            } else {
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                self(self, nodes[i].left, l, m);
                self(self, nodes[i].right, m + 1, r);
            }
        };
        driver(driver, version, 0, n - 1);
    }

    value_type range_query(int version, int l, int r) {
        value_type ans = monoid.id();
        visit_range(version, l, r, [&ans, this](value_type &val) {
            ans = monoid.op(ans, val);
        });
        return ans;
    }
};

template <algebra::Monoid M>
struct persistent_segment_tree : public basic_persistent_segment_tree<M, false> {
    using basic_persistent_segment_tree<M, false>::basic_persistent_segment_tree;
};

} // namespace range_query
