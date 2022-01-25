#pragma once

#include <type_traits>
#include <vector>
#include <cassert>

template <typename monoid, typename lazy_t = void>
struct segment_tree {
    using value_t = typename monoid::value_type;

    static constexpr auto lazy = !std::is_void<lazy_t>::value;

    int n;
    std::vector<value_t> t;

    struct empty_type { };
    [[no_unique_address]] std::conditional_t<lazy, std::vector<lazy_t>, empty_type> tag;

    virtual std::enable_if_t<lazy, void>
    apply(const lazy_t &lazy_val, int i, int l, int r) { }

    virtual std::enable_if_t<lazy, void>
    reset_tag(lazy_t &lazy_val) { }

    virtual std::enable_if_t<lazy, void>
    push_down(int i, int l, int r) {
        int m = (l + r) / 2;
        apply(tag[i], i << 1, l, m);
        apply(tag[i], i << 1 | 1, m + 1, r);
        reset_tag(tag[i]);
    }

    segment_tree() : n() { }

    template <typename iterator_t>
    segment_tree(iterator_t first, iterator_t last) {
        n = std::distance(first, last);
        t.assign(4 * n, monoid::e());
        auto build = [&](int i, int l, int r, auto &&self) -> void {
            if (l == r) {
                t[i] = value_t(*first);
                ++first;
            } else {
                int m = (l + r) / 2;
                self(i << 1, l, m, self);
                self(i << 1 | 1, m + 1, r, self);
                t[i] = monoid::f(t[i << 1], t[i << 1 | 1]);
            }
        };
        build(1, 0, n - 1, build);
    }

    template <bool update = false>
    void visit_point(int qi, auto &&f) {
        assert(0 <= qi && qi < n);
        auto visit = [&](int i, int l, int r, auto &&self) -> void {
            if (l == r) {
                f(t[i]);
            } else {
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                if (qi <= m) {
                    self(i << 1, l, m, self);
                } else {
                    self(i << 1 | 1, m + 1, r, self);
                }
                if constexpr (update) {
                    t[i] = monoid::f(t[i << 1], t[i << 1 | 1]);
                }
            }
        };
        visit(1, 0, n - 1, visit);
    }

    template <bool update = false>
    void split_range(int ql, int qr, auto &&f) {
        assert(0 <= ql && ql <= qr && qr < n);
        auto split = [&](int i, int l, int r, auto &&self) -> void {
            if (qr < l || r < ql) {

            } else if (ql <= l && r <= qr) {
                f(i, l, r);
            } else {
                if constexpr (lazy) {
                    push_down(i, l, r);
                }
                int m = (l + r) / 2;
                self(i << 1, l, m, self);
                self(i << 1 | 1, m + 1, r, self);
                if constexpr (update) {
                    t[i] = monoid::f(t[i << 1], t[i << 1 | 1]);
                }
            }
        };
        split(1, 0, n - 1, split);
    }

    value_t range_query(int l, int r) {
        value_t ans = monoid::e();
        split_range<false>(l, r, [&ans, this](int i, auto&&...) {
            ans = monoid::f(ans, t[i]);
        });
        return ans;
    }

    void point_assign(int p, const value_t &x) {
        visit_point<true>(p, [&x](value_t &y) {
            y = x;
        });
    }
};