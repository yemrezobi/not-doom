#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <ostream>
#include <cmath>

template<std::floating_point T>
class Vector3 {
public:
    T x;
    T y;
    T z;

    Vector3() noexcept : x{0}, y{0}, z{0}
    {
    }

    Vector3(T x, T y, T z) noexcept : x{x}, y{y}, z{z}
    {
    }

    auto normalized() const -> Vector3<T>
    {
        const T sum_of_squares = x * x + y * y + z * z;
        if (sum_of_squares == 0) {
            return *this;
        }
        const T magnitude = std::sqrt(sum_of_squares);
        return {x / magnitude, y / magnitude, z / magnitude};
    }

    auto dot(const Vector3<T>& rhs) const -> T
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    auto operator-() const -> Vector3<T>
    {
        return {-x, -y, -z};
    }

    // operations with scalars

    auto operator+(const T rhs) const -> Vector3<T>
    {
        return {x + rhs, y + rhs, z + rhs};
    }

    auto operator+=(const T rhs) -> Vector3<T>&
    {
        *this = *this + rhs;
        return *this;
    }

    auto operator-(const T rhs) const -> Vector3<T>
    {
        return *this + -rhs;
    }

    auto operator-=(const T rhs) -> Vector3<T>&
    {
        *this = *this - rhs;
        return *this;
    }

    auto operator*(const T rhs) const -> Vector3<T>
    {
        return {x * rhs, y * rhs, z * rhs};
    }

    auto operator*=(const T rhs) -> Vector3<T>&
    {
        *this = *this * rhs;
        return *this;
    }

    auto operator/(const T rhs) const -> Vector3<T>
    {
        return *this * (1 / rhs);
    }

    auto operator/=(const T rhs) -> Vector3<T>&
    {
        *this = *this / rhs;
        return *this;
    }

    // operations with vectors

    auto operator+(const Vector3<T>& rhs) const -> Vector3<T>
    {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    auto operator+=(const Vector3& rhs) -> Vector3<T>&
    {
        *this = *this + rhs;
        return *this;
    }

    auto operator-(const Vector3& rhs) const -> Vector3<T>
    {
        return *this + -rhs;
    }

    auto operator-=(const Vector3& rhs) -> Vector3<T>&
    {
        *this = *this - rhs;
        return *this;
    }

    // element-wise multiplication
    auto element_wise(const Vector3& rhs) -> Vector3<T>
    {
        return {x * rhs.x, y * rhs.y, z * rhs.z};
    }

    // needed for googletest output
    friend auto PrintTo(const Vector3& vector, std::ostream* os) -> void
    {
        *os << "{" << vector.x << ", " << vector.y << ", " << vector.z << "}";
    }
};

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

#endif // VECTOR3_HPP
