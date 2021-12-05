#pragma once

#include <functional>
#include <vector>
#include <limits>

template<typename range_t, typename result_t = range_t>
struct li_chao_tree {
    static_assert(std::is_floating_point<range_t>::value, "Range type must be floating-point");

    using F = std::function<result_t(range_t)>;

    static F default_f;

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
    range_t epsilon;

    li_chao_tree() {}

    li_chao_tree(range_t min_l, range_t max_r, range_t epsilon = range_t(1)) {
        assert(max_r - min_l >= epsilon);
        this->min_l = min_l;
        this->max_r = max_r;
        this->epsilon = epsilon;
        t.emplace_back();
    }

    void insert_function(F f) {
        int i = 0;
        range_t l = min_l, r = max_r;

        while (r - l >= epsilon) {
            range_t m = (l + r) / 2;
            bool left = f(l) < t[i].f(l);
            bool mid = f(m) < t[i].f(m);
            if (mid) {
                std::swap(f, t[i].f);
            }
            if (left ^ mid) {
                if (m - l >= epsilon && t[i].left == -1) {
                    t[i].left = new_node();
                }
                i = t[i].left;
                r = m;
            } else {
                if (r - m >= epsilon && t[i].right == -1) {
                    t[i].right = new_node();
                }
                i = t[i].right;
                l = m;
            }
        }
    }

    result_t get_minimum(range_t x) {
        result_t result = std::numeric_limits<result_t>::max();

        int i = 0;
        range_t l = min_l, r = max_r;

        while (i != -1) {
            result = std::min(result, t[i].f(x));
            range_t m = (l + r) / 2;
            if (m - x > epsilon) {
                i = t[i].left;
                r = m;
            } else {
                i = t[i].right;
                l = m;
            }
        }

        return result;
    }
};

template<typename range_t, typename result_t>
typename li_chao_tree<range_t, result_t>::F li_chao_tree<range_t, result_t>::default_f = [](range_t x) -> result_t {
    return std::numeric_limits<result_t>::max();
};