#include <concepts>
#include <functional>

#include "../concepts.hpp"

namespace algebra {

namespace semigroups {

template <typename T, std::strict_weak_order<T, T> C = std::less<>>
struct extremum {
    using value_type = T;

    C compare;

    extremum(C compare) : compare(compare) {}

    extremum() : extremum(C()) {}

    value_type op(value_type a, value_type b) {
        return compare(a, b) ? a : b;
    }
};

} // namespace semigroups

template <typename T, typename C>
struct is_idempotent<semigroups::extremum<T, C>> : std::true_type {};

} // namespace algebra
