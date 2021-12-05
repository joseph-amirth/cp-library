#pragma once

#include "point.hpp"
#include <vector>

template<typename T>
struct polygon : std::vector<point<T>> {
    template<typename...args>
    polygon(args...A) : std::vector<point<T>>(A...) {}

    polygon(const std::initializer_list<point<T>> &l) : std::vector<point<T>>(l) {}

    int inside(const point<T> &p) const {
        bool zero = false;
        for (int i = 0; i < this->size(); i++) {
            int j = (i == 0 ? (int) this->size() - 1 : i - 1);
            auto area = signed_triangle_area(p, (*this)[j], (*this)[i]);
            if (area < 0)
                return -1;
            zero |= area == 0;
        }
        return !zero;
    }
};