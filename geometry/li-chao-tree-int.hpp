#pragma once

#include <functional>
#include <vector>
#include <limits>

template<typename range_t, typename result_t = range_t>
struct li_chao_tree {
    static_assert(std::is_integral<range_t>::value, "Range type must be integral");

    using F = std::function<result_t(range_t)>;

    static const F default_f;

    struct node {
        F f;
        int left, right;

        node(F f = default_f) : f(f), left(-1), right(-1) {}
    };

    std::vector<node> t;

    int new_node() {
        int i = (int) t.size();
        t.emplace_back();
        return i;
    }

    range_t min_l, max_r;

    li_chao_tree() {}

    li_chao_tree(range_t min_l, range_t max_r) {
        assert(min_l <= max_r);
        this->min_l = min_l;
        this->max_r = max_r;
        new_node();
    }

    void insert_function(F f) {
        int i = 0;
        range_t l = min_l, r = max_r;

        while (true) {
            range_t m = l + (r - l) / 2;
            bool left = f(l) < t[i].f(l);
            bool mid = f(m) < t[i].f(m);
            if (mid) {
                std::swap(f, t[i].f);
            }
            if (l == r) {
                break;
            }
            if (left ^ mid) {
                if (t[i].left == -1) {
                    t[i].left = new_node();
                }
                i = t[i].left;
                r = m - 1;
            } else {
                if (t[i].right == -1) {
                    t[i].right = new_node();
                }
                i = t[i].right;
                l = m + 1;
            }
        }
    }

    result_t get_minimum(range_t x) {
        result_t result = std::numeric_limits<result_t>::max();

        int i = 0;
        range_t l = min_l, r = max_r;

        while (i != -1) {
            result = std::min(result, t[i].f(x));
            range_t m = l + (r - l) / 2;
            if (x < m) {
                i = t[i].left;
                r = m - 1;
            } else if (x > m) {
                i = t[i].right;
                l = m + 1;
            } else {
                break;
            }
        }

        return result;
    }
};

template<typename range_t, typename result_t>
const typename li_chao_tree<range_t, result_t>::F li_chao_tree<range_t, result_t>::default_f = [](
        range_t x) -> result_t {
    return std::numeric_limits<result_t>::max();
};

template<typename range_t, typename result_t = range_t>
struct li_chao_line_tree {
    static_assert(std::is_integral<range_t>::value, "Range type must be integral");

    struct line {
        result_t a, b;

        constexpr line(result_t a, result_t b) : a(a), b(b) {}

        result_t operator()(range_t x) const {
            return a * x + b;
        }

        bool operator==(const line &other) const {
            return a == other.a && b == other.b;
        }
    };

    static constexpr line default_f = line(0, std::numeric_limits<result_t>::max());

    struct node {
        line f;
        int left, right;

        node(line f = default_f) : f(f), left(-1), right(-1) {}
    };

    std::vector<node> t;

    int new_node() {
        int i = (int) t.size();
        t.emplace_back();
        return i;
    }

    range_t min_l, max_r;

    li_chao_line_tree() {}

    li_chao_line_tree(range_t min_l, range_t max_r) {
        assert(min_l <= max_r);
        this->min_l = min_l;
        this->max_r = max_r;
        new_node();
    }

    void insert_function(result_t a, result_t b) {
        auto f = line(a, b);

        int i = 0;
        range_t l = min_l, r = max_r;

        while (true) {
            range_t m = l + (r - l) / 2;
            bool left = f(l) < t[i].f(l);
            bool mid = f(m) < t[i].f(m);
            if (mid) {
                std::swap(f, t[i].f);
            }
            if (l == r || f == default_f) {
                break;
            }
            if (left ^ mid) {
                if (t[i].left == -1) {
                    t[i].left = new_node();
                }
                i = t[i].left;
                r = m - 1;
            } else {
                if (t[i].right == -1) {
                    t[i].right = new_node();
                }
                i = t[i].right;
                l = m + 1;
            }
        }
    }

    result_t get_minimum(range_t x) {
        result_t result = std::numeric_limits<result_t>::max();

        int i = 0;
        range_t l = min_l, r = max_r;

        while (i != -1) {
            result = std::min(result, t[i].f(x));
            range_t m = l + (r - l) / 2;
            if (x < m) {
                i = t[i].left;
                r = m - 1;
            } else if (x > m) {
                i = t[i].right;
                l = m + 1;
            } else {
                break;
            }
        }

        return result;
    }
};