#pragma once

#include <functional>

namespace data_structures {

template <typename Key, typename Value, typename Compare = std::less<>>
struct heap;

template <typename Key, typename Compare>
struct heap<Key, void, Compare> {

};

}
