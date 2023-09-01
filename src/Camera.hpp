#ifndef CAMERA_H
#define CAMERA_H

#include <Vec3.hpp>
#include <Vec2.hpp>
#include <array>
#include <PhysicsComponent.hpp>

class Camera {
    public:
        Camera(PhysicsComponent& physics, const double& width, const double& height);
        auto TransformToView(const Vec3& vector) const -> Vec2;

    private:
        PhysicsComponent& physics_;
        double width_;
        double height_;
};

#endif // CAMERA_H
