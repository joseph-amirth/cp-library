#pragma once

#include <concepts>
namespace data_structures {

template <typename T, typename K>
concept heap = requires(T heap, K key) {
    { heap.insert(key) };
    { heap.find_minimum() } -> std::same_as<K>;
    { heap.extract_minimum() } -> std::same_as<K>;
};

template <typename T, typename K>
concept mergeable_heap = heap<T, K> && requires(T heap1, T &&heap2) {
    { heap1.merge(heap2) } -> std::same_as<void>;
};

template <typename T, typename K, typename H>
concept addressable_heap = heap<T, K> && requires(T heap, H handle) {
    { heap.decrease_key(handle) };
};

} // namespace data_structures
