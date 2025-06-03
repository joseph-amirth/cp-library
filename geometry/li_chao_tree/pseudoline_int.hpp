#pragma once

#include <concepts>
#include <functional>
#include <utility>
#include <vector>

namespace geometry {

template <std::integral T, std::integral R = T>
struct li_chao_tree {
    // TODO: Avoid using std::function somehow.
    using F = std::function<R(T)>;

    static const F default_f;

    struct node {
        F f;
        int left, right;

        node(F f = default_f) : f(f), left(-1), right(-1) {}
    };

    std::vector<node> t;

    int new_node() {
        int i = (int)t.size();
        t.emplace_back();
        return i;
    }

    T min_l, max_r;

    li_chao_tree() {}

    li_chao_tree(T min_l, T max_r) {
        assert(min_l <= max_r);
        this->min_l = min_l;
        this->max_r = max_r;
        new_node();
    }

    void insert_function(F f) {
        int i = 0;
        T l = min_l, r = max_r;

        while (true) {
            T m = l + (r - l) / 2;
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

    R get_minimum(T x) {
        R result = std::numeric_limits<R>::max();

        int i = 0;
        T l = min_l, r = max_r;

        while (i != -1) {
            result = std::min(result, t[i].f(x));
            T m = l + (r - l) / 2;
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

template <std::integral T, std::integral R>
const typename li_chao_tree<T, R>::F li_chao_tree<T, R>::default_f = [](T x) -> R {
    return std::numeric_limits<R>::max();
};

} // namespace geometry
