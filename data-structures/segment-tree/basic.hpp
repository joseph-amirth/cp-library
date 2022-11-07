#pragma once

#include <vector>
#include <cassert>

namespace data_structures {

template <typename Groupoid, bool lazy = false>
struct basic_segment_tree {
    using groupoid_type = Groupoid;
    using value_type = typename groupoid_type::value_type;

    int n;
    std::vector<value_type> t;

    virtual void push_down(int i, int l, int r) {}

    basic_segment_tree() : n() {}

    template <typename Function>
    void build(Function &&f) {
        auto build = [&](auto &&self, int i, int l, int r) -> void {
            if (l == r) {
                f(t[i]);
            } else {
                int m = (l + r) / 2;
                self(self, i << 1, l, m);
                self(self, i << 1 | 1, m + 1, r);
                t[i] = groupoid_type::op(t[i << 1], t[i << 1 | 1]);
            }
        };
        build(build, 1, 0, n - 1);
    }

    template <typename Function>
    basic_segment_tree(int n, Function &&f) : n(n) {
        t.assign(4 * n, groupoid_type::e());
        build(f);
    }

    template <typename Iterator>
    basic_segment_tree(Iterator first, Iterator last) {
        n = std::distance(first, last);
        t.assign(4 * n, groupoid_type::e());
        build([&first](value_type &leaf) {
            leaf = value_type(*first);
            ++first;
        });
    }

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
                    t[i] = groupoid_type::op(t[i << 1], t[i << 1 | 1]);
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
                    t[i] = groupoid_type::op(t[i << 1], t[i << 1 | 1]);
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

    value_type range_query(int l, int r) {
        value_type ans = groupoid_type::e();
        split_range<false>(l, r, [&ans, this](int i, ...) {
            ans = groupoid_type::op(ans, t[i]);
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
    template <typename...Args>
    segment_tree(Args &&...args) : basic_segment_tree<M, false>(args...) {}
};

}
