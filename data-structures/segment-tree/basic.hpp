#pragma once

#include <vector>
#include <cassert>

namespace data_structures {

template <typename Monoid, bool lazy = false>
struct basic_segment_tree {
    using monoid_type = Monoid;
    using value_type = typename monoid_type::value_type;

    int n;
    std::vector<value_type> t;

    virtual void push_down(int i, int l, int r) {}

    basic_segment_tree() : n() {}

    template <typename F>
    void build(F &&f) {
        auto build = [&](auto &&self, int i, int l, int r) -> void {
            if (l == r) {
                f(t[i]);
            } else {
                int m = (l + r) / 2;
                self(self, i << 1, l, m);
                self(self, i << 1 | 1, m + 1, r);
                t[i] = monoid_type::op(t[i << 1], t[i << 1 | 1]);
            }
        };
        build(build, 1, 0, n - 1);
    }

    template <typename F>
    basic_segment_tree(int n, F &&f) : n(n) {
        t.assign(4 * n, monoid_type::e());
        build(f);
    }

    template <typename iterator_t>
    basic_segment_tree(iterator_t first, iterator_t last) {
        n = std::distance(first, last);
        t.assign(4 * n, monoid_type::e());
        build([&first](value_type &leaf) {
            leaf = value_type(*first);
            ++first;
        });
    }

    template <bool update = false>
    void visit_point(int qi, auto &&f) {
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
                    t[i] = monoid_type::op(t[i << 1], t[i << 1 | 1]);
                }
            }
        };
        visit(visit, 1, 0, n - 1);
    }

    template <bool update = false>
    void split_range(int ql, int qr, auto &&f) {
        assert(0 <= ql && ql <= qr && qr < n);
        auto split = [&](auto &&self, int i, int l, int r) -> void {
            if (qr < l || r < ql) {

            } else if (ql <= l && r <= qr) {
                f(i, l, r);
            } else {
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                self(self, i << 1, l, m);
                self(self, i << 1 | 1, m + 1, r);
                if constexpr (update) {
                    t[i] = monoid_type::op(t[i << 1], t[i << 1 | 1]);
                }
            }
        };
        split(split, 1, 0, n - 1);
    }

    value_type range_query(int l, int r) {
        value_type ans = monoid_type::e();
        split_range<false>(l, r, [&ans, this](int i, auto &&...) {
            ans = monoid_type::op(ans, t[i]);
        });
        return ans;
    }

    void point_assign(int p, const value_type &x) {
        visit_point<true>(p, [&x](value_type &y) {
            y = x;
        });
    }
};

template <typename M>
struct segment_tree : public basic_segment_tree<M, false> {
    segment_tree(auto &&...args) : basic_segment_tree<M, false>(args...) {}
};

}
