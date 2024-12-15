#pragma once

#include <array>
#include <format>
#include <numbers>

#include "Quaternion.hpp"

template<std::floating_point T>
class Matrix4x4 {
public:
    std::array<T, 16> data;

    auto operator[](const size_t index) -> T&
    {
        return data[index];
    }

    auto static constexpr identity() -> Matrix4x4<T>
    {
        return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    }

    // assumes normalized q
    auto static constexpr from_quaternion(const Quaternion<T>& q) -> Matrix4x4<T>
    {
        // clang-format off
        return {
            1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y - 2 * q.w * q.z    , 2 * q.x * q.z + 2 * q.w * q.y    , 0,
            2 * q.x * q.y + 2 * q.w * q.z    , 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z - 2 * q.w * q.x    , 0,
            2 * q.x * q.z - 2 * q.w * q.y    , 2 * q.y * q.z + 2 * q.w * q.x    , 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
            0                                , 0                                , 0                                , 1
        };
        // clang-format on
    }

    //auto static to_ndc(const)

//    auto static perspective_matrix(const double near, const double far, const double aspect_ratio, const double fov) -> Matrix4x4<T>
//    {
//        const double inv_tan = std::tan(fov * (std::numbers::pi / 180) / 2);
//        const double temp = -1 / (far - near);
//        // clang-format off
//        return {
//            inv_tan * (1 / aspect_ratio), 0, 0, 0,
//            0 , inv_tan, 0, 0,
//            0 , 0, (far + near) * temp, (2 * far * near) * temp,
//            0 , 0, -1, 0
//        };
//        // clang-format on
//    }
//
    auto static perspective_matrix(const double near, const double far, const double aspect_ratio, const double fov) -> Matrix4x4<T>
    {
        (void)near;
        (void)far;
        (void)aspect_ratio;
        (void)fov;
        //TODO: division by zero
        const double inv_tan = std::tan(fov * (std::numbers::pi / 180) / 2);
        const double temp = 1 / (far - near);
        // clang-format off
        return {
            400 * inv_tan / aspect_ratio, 0, 0, 0,
            0, 400 * inv_tan, 0, 0,
            0, 0, (far + near) * temp, (2 * far * near) * temp,
            0, 0, 1, 0
        };
        // clang-format on
    }

    auto at(const int row_index, const int column_index) const -> const T&
    {
        return data[row_index * 4 + column_index];
    }

    auto at(const int row_index, const int column_index) -> T&
    {
        return data[row_index * 4 + column_index];
    }

    auto operator-() const -> Matrix4x4<T>
    {
        Matrix4x4<T> result{};
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                result.at(row, col) = -this->at(row, col);
            }
        }
        return result;
    }

    // operations with scalars
    auto operator+(T rhs) const -> Matrix4x4<T>
    {
        Matrix4x4<T> result{};
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                result.at(row, col) = this->at(row, col) + rhs;
            }
        }
        return result;
    }

    auto operator+=(T rhs) -> Matrix4x4<T>&
    {
        *this = *this + rhs;
        return *this;
    }

    auto operator-(T rhs) const -> Matrix4x4<T>
    {
        return *this + -rhs;
    }

    auto operator-=(T rhs) -> Matrix4x4<T>&
    {
        *this = *this - rhs;
        return *this;
    }

    auto operator*(T rhs) const -> Matrix4x4<T>
    {
        Matrix4x4<T> result{};
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                result.at(row, col) = this->at(row, col) * rhs;
            }
        }
        return result;
    }

    auto operator*=(T rhs) -> Matrix4x4<T>&
    {
        *this = *this * rhs;
        return *this;
    }

    auto operator/(T rhs) const -> Matrix4x4<T>
    {
        return *this * (1 / rhs);
    }

    auto operator/=(T rhs) -> Matrix4x4<T>&
    {
        *this = *this / rhs;
        return *this;
    }

    // operations with matrices

    auto operator+(const Matrix4x4<T>& rhs) const -> Matrix4x4<T>
    {
        Matrix4x4<T> result{};
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                result.at(row, col) = this->at(row, col) + rhs.at(row, col);
            }
        }
        return result;
    }

    auto operator+=(const Matrix4x4<T>& rhs) -> Matrix4x4<T>&
    {
        *this = *this + rhs;
        return *this;
    }

    auto operator-(const Matrix4x4<T>& rhs) const -> Matrix4x4<T>
    {
        return *this + -rhs;
    }

    auto operator-=(const Matrix4x4<T>& rhs) -> Matrix4x4<T>&
    {
        *this = *this - rhs;
        return *this;
    }

    auto operator*(const Matrix4x4<T>& rhs) const -> Matrix4x4<T>
    {
        Matrix4x4<T> result{};
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                for (int i = 0; i < 4; i++) {
                    result.at(row, col) += this->at(row, i) * rhs.at(i, col);
                }
            }
        }
        return result;
    }

    auto operator*=(const Matrix4x4<T>& rhs) -> Matrix4x4<T>&
    {
        *this = *this * rhs;
        return *this;
    }

    // apply rotation to vector
    auto rotate_point(const Vector3<T>& rhs) const -> Vector3<T>
    {
        // clang-format off
        const Quaternion<T> temp{
            this->at(3, 0) * rhs.x + this->at(3, 1) * rhs.y + this->at(3, 2) * rhs.z + this->at(3, 3),
            this->at(0, 0) * rhs.x + this->at(0, 1) * rhs.y + this->at(0, 2) * rhs.z + this->at(0, 3),
            this->at(1, 0) * rhs.x + this->at(1, 1) * rhs.y + this->at(1, 2) * rhs.z + this->at(1, 3),
            this->at(2, 0) * rhs.x + this->at(2, 1) * rhs.y + this->at(2, 2) * rhs.z + this->at(2, 3)
        };
        // clang-format on

        if(temp.w == 0) { // improper input matrix
            return {};
        }
        return {temp.x / temp.w, temp.y / temp.w, temp.z / temp.w};
    }

    auto operator*(const Vector3<T>& rhs) const -> Vector3<T>
    {
        return this->rotate_point(rhs);
    }

    // needed for googletest output
    friend auto PrintTo(const Matrix4x4<T>& mat, std::ostream* os) -> void
    {
        *os << std::format(
            "\n[{:> 10.3f}, {:> 10.3f}, {:> 10.3f}, {:> 10.3f}]\n"
            "[{:> 10.3f}, {:> 10.3f}, {:> 10.3f}, {:> 10.3f}]\n"
            "[{:> 10.3f}, {:> 10.3f}, {:> 10.3f}, {:> 10.3f}]\n"
            "[{:> 10.3f}, {:> 10.3f}, {:> 10.3f}, {:> 10.3f}]",
            mat.data[0], mat.data[1], mat.data[2], mat.data[3], mat.data[4], mat.data[5], mat.data[6], mat.data[7],
            mat.data[8], mat.data[9], mat.data[10], mat.data[11], mat.data[12], mat.data[13], mat.data[14],
            mat.data[15]);
    }
};

using Matrix4x4f = Matrix4x4<float>;
using Matrix4x4d = Matrix4x4<double>;
