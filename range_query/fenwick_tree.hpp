#include "../algebra/concepts.hpp"
#include <iterator>

namespace range_query {

template <algebra::Monoid M>
struct fenwick_tree {
    using value_type = typename M::value_type;

    M monoid;
    std::size_t n;
    value_type *data;

    fenwick_tree(M monoid, std::size_t n) : monoid(monoid), n(n), data(new value_type[n + 1]) {
        std::fill(data + 1, data + n + 1, monoid.id());
    }

    fenwick_tree(std::size_t n) : fenwick_tree(M(), n) {}

    template <std::forward_iterator Iterator>
    fenwick_tree(M monoid, Iterator first, Iterator last) : fenwick_tree(monoid, std::distance(first, last)) {
        for (std::size_t i = 0; i < n; i++) {
            this->apply(i, *first++);
        }
    }

    template <std::forward_iterator Iterator>
    fenwick_tree(Iterator first, Iterator last) : fenwick_tree(M(), first, last) {}

    value_type query(std::size_t i) {
        value_type result = monoid.id();
        for (i += 1; i > 0; i -= i & -i) {
            result = monoid.op(data[i], result);
        }
        return result;
    }

    void apply(std::size_t i, value_type x) {
        for (i += 1; i <= n; i += i & -i) {
            data[i] = monoid.op(data[i], x);
        }
    }

    value_type range_query(std::size_t l, std::size_t r)
        requires algebra::Group<M>
    {
        return monoid.op(query(r), monoid.inv(l != 0 ? query(l - 1) : monoid.id()));
    }

    fenwick_tree(const fenwick_tree &) = delete;

    ~fenwick_tree() {
        delete[] data;
    }
};

} // namespace range_query
