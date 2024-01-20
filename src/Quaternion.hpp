#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <cmath>
#include <ostream>

#include "Vector3.hpp"

template<std::floating_point T>
class Quaternion {
public:
    T w;
    T x;
    T y;
    T z;

    Quaternion() noexcept : w{0}, x{0}, y{0}, z{0}
    {
    }

    Quaternion(T w, T x, T y, T z) noexcept : w{w}, x{x}, y{y}, z{z}
    {
    }

    auto static identity() -> Quaternion<T>
    {
        return {1, 0, 0, 0};
    }

    auto static from_axis_angle(const Vector3<T>& axis, T angle) -> Quaternion<T>
    {
        const T sine = std::sin(angle / 2);
        const T cosine = std::cos(angle / 2);

        Quaternion<T> temp = {cosine, axis.x * sine, axis.y * sine, axis.z * sine};
        return temp.normalized();
    }

    auto normalized() const -> Quaternion<T>
    {
        const T sum_of_squares = w * w + x * x + y * y + z * z;
        if (sum_of_squares == 0) {
            return *this;
        }
        const T magnitude = std::sqrt(sum_of_squares);
        return {w / magnitude, x / magnitude, y / magnitude, z / magnitude};
    }

    auto inverse() const -> Quaternion<T>
    {
        const Quaternion<T> normalized = this->normalized();
        return {this->w, -normalized.x, -normalized.y, -normalized.z};
    }

    auto dot(const Quaternion<T>& rhs) const -> T
    {
        return this->w * rhs.w + this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
    }

    // apply quaternion rotation to vector
    auto rotate_point(const Vector3<T>& vector) const -> Vector3<T>
    {
        Quaternion<T> temp = Quaternion<T>{0, vector.x, vector.y, vector.z} * this->inverse();
        temp = this->normalized() * temp;
        return {temp.x, temp.y, temp.z};
    }

    // for t in 0 < t < 1
    auto slerp_to(const Quaternion<T>& rhs, const T t) const -> Quaternion<T>
    {
        const T theta = std::acos(this->dot(rhs));

        return *this * std::sin((1 - t) * theta) / std::sin(theta) + rhs * std::sin(t * theta) / std::sin(theta);
    }

    auto operator+(const Quaternion<T>& rhs) const -> Quaternion<T>
    {
        return {this->w + rhs.w, this->x + rhs.x, this->y + rhs.y, this->z + rhs.z};
    }

    auto operator+=(const Quaternion<T>& rhs) -> Quaternion<T>&
    {
        *this = *this + rhs;
        return *this;
    }

    auto operator/(const T rhs) const -> Quaternion<T>
    {
        return {this->w / rhs, this->x / rhs, this->y / rhs, this->z / rhs};
    }

    auto operator/=(const T rhs) -> Quaternion<T>&
    {
        *this = *this / rhs;
        return *this;
    }

    auto operator*(const T rhs) const -> Quaternion<T>
    {
        return {this->w * rhs, this->x * rhs, this->y * rhs, this->z * rhs};
    }

    auto operator*=(const T rhs) -> Quaternion<T>&
    {
        *this = *this * rhs;
        return *this;
    }

    auto operator*(const Quaternion<T>& rhs) const -> Quaternion<T>
    {
        const T w1 = this->w;
        const T x1 = this->x;
        const T y1 = this->y;
        const T z1 = this->z;

        const T w2 = rhs.w;
        const T x2 = rhs.x;
        const T y2 = rhs.y;
        const T z2 = rhs.z;

        const T w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
        const T x = w1 * x2 + w2 * x1 + y1 * z2 - z1 * y2;
        const T y = w1 * y2 + w2 * y1 + z1 * x2 - x1 * z2;
        const T z = w1 * z2 + w2 * z1 + x1 * y2 - y1 * x2;
        return {w, x, y, z};
    }

    auto operator*=(const Quaternion<T>& rhs) -> Quaternion<T>&
    {
        *this = *this * rhs;
        return *this;
    }

    // needed for googletest output
    friend auto PrintTo(const Quaternion<T>& q, std::ostream* os) -> void
    {
        *os << "{" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << "}";
    }
};

using Quaternionf = Quaternion<float>;
using Quaterniond = Quaternion<double>;

#endif // QUATERNION_HPP
