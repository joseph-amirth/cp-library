#pragma once

#include <tuple>

template <typename...Iterator>
struct zip_iterator {
    std::tuple<Iterator...> t;

    zip_iterator(Iterator...its) : t(its...) {}

    zip_iterator(std::tuple<Iterator...> t) : t(t) {}

    auto operator*() {
        return std::apply([](Iterator...its) {
            return std::make_tuple(*its...);
        }, t);
    }

    bool operator!=(const zip_iterator &other) {
        return t != other.t;
    }

    zip_iterator operator++() {
        return std::apply([](auto &...its) {
            return std::make_tuple(its++...);
        }, t);
    }
};

template <typename...Iterator>
struct zip_range {
    using iterator_type = zip_iterator<Iterator...>;

    iterator_type begin_;
    iterator_type end_;

    zip_range(Iterator...begins, Iterator...ends) : begin_(begins...), end_(ends...) {}

    template <typename...Range>
    zip_range(Range &&...range) : zip_range(range.begin()..., range.end()...) {}

    iterator_type begin() const {
        return begin_;
    }

    iterator_type end() const {
        return end_;
    }
};

template <typename...Range>
auto zip(Range &&...range) {
    return zip_range<decltype(range.begin())...>(std::forward<Range>(range)...);
}
