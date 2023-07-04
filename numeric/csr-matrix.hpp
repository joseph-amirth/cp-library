#pragma once

#include <vector>
#include <ranges>
#include <numeric>
#include <cassert>
#include "../miscellaneous/zip.hpp"

template <typename T>
struct csr_matrix {
    using value_type = T;

    int m, n;

    value_type default_value;
    std::vector<value_type> values;

    std::vector<int> cols;
    std::vector<int> cols_ptr;

    csr_matrix(int m, int n, const value_type &default_value, std::vector<value_type> &&values, std::vector<int> &&cols,
               std::vector<int> &&cols_ptr) {
        this->m = m;
        this->n = n;
        this->default_value = this->default_value;
        this->values = std::move(values);
        this->cols = std::move(cols);
        this->cols_ptr = std::move(cols_ptr);
    }

    struct row_range : zip_range<const int *, const value_type *> {
        using zip_range<const int *, const value_type *>::begin;
        using zip_range<const int *, const value_type *>::end;

        value_type default_value;

        row_range(const int *cols_first, const value_type *values_first,
                  const int *cols_last, const value_type *values_last,
                  const value_type &default_value)
                : zip_range<const int *, const value_type *>(cols_first, values_first, cols_last, values_last),
                  default_value(default_value) {}

        const value_type &operator[](int qj) const {
            for (const auto &[j, x] : *this) {
                if (qj == j) {
                    return x;
                }
            }
            return default_value;
        }
    };

    auto operator[](int i) const {
        return row_range(cols.data() + cols_ptr[i], values.data() + cols_ptr[i],
                         cols.data() + cols_ptr[i + 1], values.data() + cols_ptr[i + 1],
                         default_value);
    }
};

template <typename T>
struct csr_matrix_builder {
    using value_type = T;

    int m, n;
    value_type default_value;
    std::vector<std::tuple<int, int, value_type>> entries;

    csr_matrix_builder(int m, int n,
                       const value_type &default_value = value_type(),
                       int n_entries = 0) :
                       m(m), n(n), default_value(default_value) {

        entries.reserve(n_entries);
    }

    void add_entry(int i, int j, const value_type &x) {
        entries.emplace_back(i, j, x);
    }

    csr_matrix<value_type> build() {
        std::vector<int> cols_ptr(m + 1);
        for (auto &[i, j, x] : entries) {
            cols_ptr[i]++;
        }

        std::exclusive_scan(cols_ptr.begin(), cols_ptr.end(), cols_ptr.begin(), 0);

        int sz = entries.size();

        std::vector<value_type> values(sz);
        std::vector<int> cols(sz), ind(m);

        for (auto &[i, j, x] : entries) {
            values[cols_ptr[i] + ind[i]] = x;
            cols[cols_ptr[i] + ind[i]] = j;
            ind[i]++;
        }

        return csr_matrix<value_type>(m, n, default_value, std::move(values), std::move(cols), std::move(cols_ptr));
    }
};
