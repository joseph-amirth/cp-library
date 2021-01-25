#include <type_traits>

template <typename T>
struct point {
    T x, y;
    point() : x(), y() { }
    point(T x, T y) : x(x), y(y) { }

    point operator+() const { return point(x, y); }
    point operator-() const { return point(-x, -y); }

    point &operator+=(const point &p) { return x += p.x, y += p.y, *this; }
    point &operator-=(const point &p) { return x -= p.x, y -= p.y, *this; }
    point &operator*=(const T &val) { return x *= val, y *= val, *this; }
    point &operator/=(const T &val) { return x /= val, y /= val, *this; }

    point operator+(const point &p) { return point(x + p.x, y + p.y); }
    point operator-(const point &p) { return point(x - p.x, y - p.y); }
    point operator*(const T &val) { return point(x * val, y * val); }
    point operator/(const T &val) { return point(x / val, y / val); }

    friend point operator*(const T &val, const point &p) { return point(val * p.x, val * p.y); }

    bool operator==(const point &p) const {
        return x == p.x && y == p.y;
    }
};