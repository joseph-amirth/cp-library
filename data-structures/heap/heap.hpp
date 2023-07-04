#pragma once

#include <functional>

namespace data_structures {

template <typename Key>
struct heap {
    using key_type = Key;

    virtual key_type extract_minimum() = 0;

    virtual void insert(const key_type &) = 0;

    template <typename...Args>
    void emplace(Args &&...args) {
        insert(key_type(std::forward<Args>(args)...));
    }
};

template <typename Key, typename Handle>
struct addressable_heap {
    using key_type = Key;
    using handle_type = Handle;

    virtual key_type extract_minimum() = 0;

    virtual handle_type insert(const key_type &) = 0;

    virtual void decrease_key(handle_type) = 0;

    template <typename...Args>
    handle_type emplace(Args &&...args) {
        return insert(key_type(std::forward<Args>(args)...));
    }
};

template <typename Heap, typename AbstractHeap>
struct mergeable_heap : AbstractHeap {
    using heap_type = Heap;

    virtual void merge(heap_type &&) = 0;
};

}
