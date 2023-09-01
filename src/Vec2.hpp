#ifndef VEC2_H
#define VEC2_H

class Vec2 {
    public:
        Vec2(double x, double y);
        static auto Zero() -> Vec2;
        auto operator+(const Vec2& rhs) -> Vec2;
        auto operator+=(const Vec2& rhs) -> Vec2&;
        double x;
        double y;
    private:
};

#endif // VEC2_H
