#include "../algebra/concepts.hpp"
#include <iterator>

namespace range_query {

template <algebra::Semigroup S>
    requires algebra::Idempotent<S>
struct sparse_table {
    using value_type = typename S::value_type;

    S semigroup;
    std::size_t n;
    value_type *table;

    template <std::forward_iterator I>
    sparse_table(S semigroup, I first, I last) : semigroup(semigroup), n(std::distance(first, last)) {
        std::size_t lg = 32 - __builtin_clz(n);
        this->table = new value_type[lg * n];
        std::copy(first, last, table);
        for (std::size_t j = 1; j < lg; j++) {
            for (std::size_t i = 0; i + (1 << j) <= n; i++) {
                table[n * j + i] = semigroup.op(table[n * (j - 1) + i], table[n * (j - 1) + i + (1 << (j - 1))]);
            }
        }
    }

    template <std::forward_iterator I>
    sparse_table(I first, I last) : sparse_table(S(), first, last) {}

    value_type range_query(std::size_t l, std::size_t r) {
        std::size_t j = 32 - __builtin_clz(r - l + 1) - 1;
        return semigroup.op(table[n * j + l], table[n * j + r + 1 - (1 << j)]);
    }

    sparse_table(const sparse_table &) = delete;

    ~sparse_table() {
        delete table;
    }
};

} // namespace range_query
