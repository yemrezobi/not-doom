#ifndef VEC3_H
#define VEC3_H

class Vec3 {
    public:
        Vec3(double x, double y, double z);
        static auto Zero() -> Vec3;
        auto operator+(const Vec3& rhs) -> Vec3;
        auto operator+=(const Vec3& rhs) -> Vec3&;
        double x;
        double y;
        double z;
    private:
};

#endif // VEC3_H
