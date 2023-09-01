#include <PhysicsComponent.hpp>
#include <Camera.hpp>

Camera::Camera(PhysicsComponent& physics, const double& width, const double& height)
    : physics_(physics)
    , width_(width)
    , height_(height)
{
}

auto Camera::TransformToView(const Vec3& vector) const -> Vec2
{
    return Vec2( width_ / 2 + vector.x - physics_.GetPosition().x, height_ / 2 - vector.y + physics_.GetPosition().y );
}
