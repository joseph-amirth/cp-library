#pragma once

#include <cstddef>

template <typename T>
struct stack_allocator {
    using value_type = T;

    std::size_t capacity;
    value_type *data, *top;

    stack_allocator(std::size_t n = 0) : capacity(n), data(new value_type[capacity]), top(data) {}

    template <typename U>
    stack_allocator(const stack_allocator<U> &other) : stack_allocator(other.capacity) {}

    [[nodiscard]] T* allocate(std::size_t n) {
        T* p = top;
        top += n;
        return p;
    }

    void deallocate(T* p, std::size_t n) {
        top -= n;
        assert(p == top);
    }

    ~stack_allocator() {
        assert(top == data);
        delete[] data;
    }
};
