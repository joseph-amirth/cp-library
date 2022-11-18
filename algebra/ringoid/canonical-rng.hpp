#pragma once

namespace algebra {

template <typename T>
struct canonical_rng {
    using value_type = T;

    static value_type zero() {
        return T(0);
    }

    static value_type sum(const value_type &x, const value_type &y) {
        return x + y;
    }

    static value_type negation(const value_type &x) {
        return -x;
    }

    static value_type product(const value_type &x, const value_type &y) {
        return x * y;
    }
};

}
