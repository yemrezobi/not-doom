#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include <ostream>

template<std::floating_point T>
class Vector2 {
public:
    T x;
    T y;

    Vector2() noexcept : x{0}, y{0}
    {
    }

    Vector2(T x, T y) noexcept : x{x}, y{y}
    {
    }

    auto normalized() const -> Vector2<T>
    {
        const T sum_of_squares = x * x + y * y;
        if (sum_of_squares == 0) {
            return *this;
        }
        const T magnitude = std::sqrt(sum_of_squares);
        return {x / magnitude, y / magnitude};
    }

    auto dot(const Vector2<T>& rhs) const -> T
    {
        return x * rhs.x + y * rhs.y;
    }

    auto operator-() const -> Vector2<T>
    {
        return {-x, -y};
    }

    // operations with scalars

    auto operator+(const T rhs) const -> Vector2<T>
    {
        return {x + rhs, y + rhs};
    }

    auto operator+=(const T rhs) -> Vector2<T>&
    {
        *this = *this + rhs;
        return *this;
    }

    auto operator-(const T rhs) const -> Vector2<T>
    {
        return *this + -rhs;
    }

    auto operator-=(const T rhs) -> Vector2<T>&
    {
        *this = *this - rhs;
        return *this;
    }

    auto operator*(const T rhs) const -> Vector2<T>
    {
        return {x * rhs, y * rhs};
    }

    auto operator*=(const T rhs) -> Vector2<T>&
    {
        *this = *this * rhs;
        return *this;
    }

    auto operator/(const T rhs) const -> Vector2<T>
    {
        return *this * (1 / rhs);
    }

    auto operator/=(const T rhs) -> Vector2<T>&
    {
        *this = *this / rhs;
        return *this;
    }

    // operations with vectors

    auto operator+(const Vector2<T>& rhs) const -> Vector2<T>
    {
        return {x + rhs.x, y + rhs.y};
    }

    auto operator+=(const Vector2<T>& rhs) -> Vector2<T>
    {
        *this = *this + rhs;
        return *this;
    }

    auto operator-(const Vector2<T>& rhs) const -> Vector2<T>
    {
        return *this + -rhs;
    }

    auto operator-=(const Vector2<T>& rhs) -> Vector2<T>&
    {
        *this = *this - rhs;
        return *this;
    }

    // needed for googletest output
    friend auto PrintTo(const Vector2<T>& vector, std::ostream* os) -> void
    {
        *os << "{" << vector.x << ", " << vector.y << "}";
    }
};

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

#endif // VECTOR2_HPP
