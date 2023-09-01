#include <Vec2.hpp>

Vec2::Vec2(double x, double y)
    : x(x), y(y)
{
}

auto Vec2::Zero() -> Vec2 {
    return { 0, 0 };
}

auto Vec2::operator+(const Vec2& rhs) -> Vec2
{
    return Vec2{ x + rhs.x, y + rhs.y };
}

auto Vec2::operator+=(const Vec2& rhs) -> Vec2&
{
    x = x + rhs.x;
    y = y + rhs.y;
    return *this;
}
