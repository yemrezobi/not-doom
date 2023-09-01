#include <Vec3.hpp>

Vec3::Vec3(double x, double y, double z)
    : x(x), y(y), z(z)
{
}

auto Vec3::Zero() -> Vec3 {
    return { 0, 0, 0 };
}

auto Vec3::operator+(const Vec3& rhs) -> Vec3
{
    return Vec3{ x + rhs.x, y + rhs.y, z + rhs.z };
}

auto Vec3::operator+=(const Vec3& rhs) -> Vec3&
{
    x = x + rhs.x;
    y = y + rhs.y;
    z = z + rhs.z;
    return *this;
}
