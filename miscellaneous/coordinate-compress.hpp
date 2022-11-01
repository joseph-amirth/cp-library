#pragma once

#include <vector>

template <typename Value>
struct coordinate_compress {
    using value_type = Value;

    int sigma;
    std::vector<value_type> values;

    coordinate_compress() : sigma(-1) {}

    coordinate_compress(auto&&...args) : sigma(-1) {
        insert(args...);
    }

    void insert(const value_type &value) {
        values.push_back(value);
    }

    template <typename Iterator>
    void insert(Iterator first, Iterator last) {
        values.insert(values.end(), first, last);
    }

    void process() {
        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
        sigma = values.size();
    }

    int compress(const value_type &value) const {
        return std::lower_bound(values.begin(), values.end(), value) - values.begin();
    }

    template <typename Iterator>
    void compress(Iterator first, Iterator last) {
        for (; first != last; first++) {
            *first = compress(*first);
        }
    }
};
