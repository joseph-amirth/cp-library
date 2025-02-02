#include "../algebra/concepts.hpp"
#include <iterator>

namespace range_query {

template <algebra::Semigroup S>
struct disjoint_sparse_table {
    using value_type = typename S::value_type;

    S semigroup;
    std::size_t n;
    value_type *table;

    template <std::forward_iterator I>
    disjoint_sparse_table(S semigroup, I first, I last) : semigroup(semigroup), n(std::distance(first, last)) {
        std::size_t lg = 32 - __builtin_clz(n);
        this->table = new value_type[lg * n];
        for (std::size_t k = 0; k < lg; k++) {
            std::copy(first, last, table + n * k);
        }
        for (std::size_t k = 1; k < lg; k++) {
            for (std::size_t j = 1 << k; j < n; j += 2 << k) {
                for (int i = j - 2; i >= (int)j - (1 << k); i--) {
                    table[n * k + i] = semigroup.op(table[i], table[n * k + i + 1]);
                }
                for (std::size_t i = j + 1; i < std::min(n, j + (1 << k)); i++) {
                    table[n * k + i] = semigroup.op(table[n * k + i - 1], table[i]);
                }
            }
        }
    }

    template <std::forward_iterator I>
    disjoint_sparse_table(I first, I last) : disjoint_sparse_table(S(), first, last) {}

    value_type range_query(std::size_t l, std::size_t r) {
        if (l != r) {
            std::size_t j = 32 - __builtin_clz(l ^ r) - 1;
            return semigroup.op(table[n * j + l], table[n * j + r]);
        } else {
            return table[l];
        }
    }

    disjoint_sparse_table(const disjoint_sparse_table &) = delete;

    ~disjoint_sparse_table() {
        delete table;
    }
};

} // namespace range_query
