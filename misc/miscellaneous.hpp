#include <type_traits>
#include <utility>
#include <array>
#include <assert.h>

template<typename T, typename F>
std::enable_if_t<std::is_floating_point<T>::value, T>
ternary_search(T lo, T hi, F f, T epsilon = 1e-9) {
    while (hi - lo > epsilon) {
        T mid1 = lo + (hi - lo) / 3, mid2 = hi - (hi - lo) / 3;
        if (f(mid1) < f(mid2)) {
            hi = mid2;
        } else {
            lo = mid1;
        }
    }
    return lo;
}

template<typename T, typename F>
std::enable_if_t<std::is_integral<T>::value, std::pair<T, T>>
ternary_search(T lo, T hi, F f) {
    while (hi - lo > 2) {
        T mid1 = lo + (hi - lo) / 3, mid2 = hi - (hi - lo) / 3;
        T f1 = f(mid1), f2 = f(mid2);

        if (f1 == f2) {
            lo = mid1, hi = mid2;
        } else if (f1 < f2) {
            hi = mid2;
        } else {
            lo = mid1;
        }
    }

    T opt = lo, opt_val = f(lo);
    for (T i = lo + 1; i <= hi; i++) {
        T f_val = f(i);
        if (f_val > opt_val) {
            opt = i;
            opt_val = f_val;
        }
    }
    return make_pair(opt, opt_val);
}

template<typename T, typename F>
std::enable_if_t<std::is_integral<T>::value, T>
binary_search(T lo, T hi, F f) {
    while (lo < hi) {
        T mid = lo + (hi - lo) / 2;
        if (f(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

template<typename T, typename F>
std::enable_if_t<std::is_floating_point<T>::value, T>
binary_search(T lo, T hi, F f, T epsilon = 1e-9) {
    while (hi - lo > epsilon) {
        T mid = (lo + hi) / 2;
        if (f(mid)) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return lo;
}

template<typename T, int N>
struct sub_maximum {
    std::array<T, N> mx;

    sub_maximum(T e) : mx() {
        mx.fill(e);
    }

    void update(T item) {
        for (int i = 0; i < N; i++) {
            if (item > mx[i]) {
                if (i + 1 != N) {
                    std::copy(mx.begin() + i, mx.end() - 1, mx.begin() + i + 1);
                }
                mx[i] = item;
                break;
            }
        }
    }

    T rank(int i) {
        assert(0 <= i && i < N);
        return mx[i];
    }
};