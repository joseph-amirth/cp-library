#pragma once

#include "algebra/concepts.hpp"

#include <cassert>
#include <concepts>
#include <iterator>
#include <vector>

namespace range_query {

template <algebra::Monoid M, bool lazy = false>
struct basic_segment_tree {
    using groupoid = M;
    using value_type = typename M::value_type;

    M monoid;
    int n;
    std::vector<value_type> t;

    virtual void push_down(int i, int l, int r) {}

    template <typename Function>
    void build(Function &&f) {
        auto build = [&](auto &&self, int i, int l, int r) -> void {
            if (l == r) {
                f(t[i]);
            } else {
                int m = (l + r) / 2;
                self(self, i << 1, l, m);
                self(self, i << 1 | 1, m + 1, r);
                t[i] = monoid.op(t[i << 1], t[i << 1 | 1]);
            }
        };
        build(build, 1, 0, n - 1);
    }

    template <std::forward_iterator I>
    basic_segment_tree(M monoid, I first, I last) : basic_segment_tree(monoid, std::distance(first, last)) {
        build([&first](value_type &leaf) {
            leaf = value_type(*first);
            ++first;
        });
    }

    template <std::forward_iterator I>
    basic_segment_tree(I first, I last) : basic_segment_tree(M(), first, last) {}

    basic_segment_tree(M monoid, int n) : monoid(monoid), n(n) {
        t.assign(4 * n, monoid.id());
    }

    basic_segment_tree(int n) : basic_segment_tree(M(), n) {}

    template <bool update = false, typename Function>
    void visit_point(int qi, Function &&f) {
        assert(0 <= qi && qi < n);
        auto visit = [&](auto &&self, int i, int l, int r) -> void {
            if (l == r) {
                f(t[i]);
            } else {
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                if (qi <= m) {
                    self(self, i << 1, l, m);
                } else {
                    self(self, i << 1 | 1, m + 1, r);
                }
                if constexpr (update) {
                    t[i] = monoid.op(t[i << 1], t[i << 1 | 1]);
                }
            }
        };
        visit(visit, 1, 0, n - 1);
    }

    template <bool update = false, typename Function>
    void walk_down(Function &&f) {
        auto walk = [&](auto &&self, int i, int l, int r) -> void {
            if (!f(i, l, r)) {
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                self(self, i << 1, l, m);
                self(self, i << 1 | 1, m + 1, r);
                if constexpr (update) {
                    t[i] = monoid.op(t[i << 1], t[i << 1 | 1]);
                }
            }
        };
        walk(walk, 1, 0, n - 1);
    }

    template <bool update = false, typename Function>
    void split_range(int ql, int qr, Function &&f) {
        assert(0 <= ql && ql <= qr && qr < n);
        walk_down<update>([&ql, &qr, &f](int i, int l, int r) {
            if (qr < l || r < ql) {
                return true;
            } else if (ql <= l && r <= qr) {
                f(i, l, r);
                return true;
            } else {
                return false;
            }
        });
    }

    int find_leftmost(auto &&f) {
        if (!f(all())) {
            return n;
        }
        auto find = [&](auto &&self, int i, int l, int r) {
            if (l == r) {
                return l;
            } else {
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                if (f(t[i << 1])) {
                    return self(self, i << 1, l, m);
                } else {
                    return self(self, i << 1 | 1, m + 1, r);
                }
            }
        };
        return find(find, 1, 0, n - 1);
    }

    int find_rightmost(auto &&f) {
        if (!f(all())) {
            return -1;
        }
        auto find = [&](auto &&self, int i, int l, int r) {
            if (l == r) {
                return l;
            } else {
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                if (f(t[i << 1 | 1])) {
                    return self(self, i << 1 | 1, m + 1, r);
                } else {
                    return self(self, i << 1, l, m);
                }
            }
        };
        return find(find, 1, 0, n - 1);
    }

    value_type range_query(int l, int r) {
        value_type ans = monoid.id();
        split_range<false>(l, r, [&ans, this](int i, ...) {
            ans = monoid.op(ans, t[i]);
        });
        return ans;
    }

    value_type all() {
        return t[1];
    }

    void point_assign(int p, const value_type &x) {
        visit_point<true>(p, [&x](value_type &y) {
            y = x;
        });
    }
};

template <algebra::Monoid M>
struct segment_tree : public basic_segment_tree<M, false> {
    using basic_segment_tree<M, false>::basic_segment_tree;
};

} // namespace range_query
