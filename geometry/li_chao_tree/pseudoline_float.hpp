#pragma once

#include <concepts>
#include <functional>

namespace geometry {

template <std::floating_point T, std::floating_point R = T>
struct li_chao_tree {
    // TODO: Avoid using std::function somehow.
    using F = std::function<R(T)>;

    static F default_f;

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
    T epsilon;

    li_chao_tree() {}

    li_chao_tree(T min_l, T max_r, T epsilon = T(1)) {
        assert(max_r - min_l >= epsilon);
        this->min_l = min_l;
        this->max_r = max_r;
        this->epsilon = epsilon;
        t.emplace_back();
    }

    void insert_function(F f) {
        int i = 0;
        T l = min_l, r = max_r;

        while (r - l >= epsilon) {
            T m = (l + r) / 2;
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

    R get_minimum(T x) {
        R result = std::numeric_limits<R>::max();

        int i = 0;
        T l = min_l, r = max_r;

        while (i != -1) {
            result = std::min(result, t[i].f(x));
            T m = (l + r) / 2;
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

template <std::floating_point T, std::floating_point R>
typename li_chao_tree<T, R>::F li_chao_tree<T, R>::default_f = [](T x) -> R {
    return std::numeric_limits<R>::max();
};

} // namespace geometry
