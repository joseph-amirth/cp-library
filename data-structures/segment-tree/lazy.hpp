#pragma once

#include "basic.hpp"

namespace data_structures {

template <typename Monoid, typename LazyUpdate>
struct lazy_segment_tree : public basic_segment_tree<Monoid, true> {
    using groupoid = Monoid;
    using lazy_update_type = LazyUpdate;
    using tag_type = typename lazy_update_type::tag_type;

    using basic_segment_tree<groupoid, true>::n;
    using basic_segment_tree<groupoid, true>::t;

    std::vector<tag_type> tag;

    template <typename...Args>
    lazy_segment_tree(Args &&...args) : basic_segment_tree<groupoid, true>(std::forward<Args>(args)...) {
        tag.assign(4 * n, lazy_update_type::e());
    }

    virtual void push_down(int i, int l, int r) {
        int m = l + (r - l) / 2;
        lazy_update_type::apply(tag[i], t[i << 1], tag[i << 1], l, m);
        lazy_update_type::apply(tag[i], t[i << 1 | 1], tag[i << 1 | 1], m + 1, r);
        tag[i] = lazy_update_type::e();
    }
};

}
