#include <PhysicsComponent.hpp>
#include <Debug.hpp>

PhysicsComponent::PhysicsComponent(Vec3 position, Vec3 velocity, Vec3 acceleration)
    : position_(position),
      velocity_(velocity),
      acceleration_(acceleration),
      is_affected_by_gravity_(false)
{}

auto PhysicsComponent::GetPosition() const -> Vec3
{
    return position_;
}

auto PhysicsComponent::SetPosition(const Vec3& position) -> void
{
    position_ = position;
}

auto PhysicsComponent::GetVelocity() const -> Vec3
{
    return velocity_;
}

auto PhysicsComponent::SetVelocity(const Vec3& velocity) -> void
{
    velocity_ = velocity;
}

auto PhysicsComponent::SetAffectedByGravity(bool val) -> void
{
    double grav = kGravitationalConstant * (static_cast<double>(val) - static_cast<double>(is_affected_by_gravity_));
    std::cout << std::format("adding G {}", grav) << std::endl;
    //acceleration_.y += grav;
    is_affected_by_gravity_ = val;
}

auto PhysicsComponent::IsOnGround() -> bool
{
    return position_.y == 0;
}

auto PhysicsComponent::Update() -> void
{
    velocity_ += acceleration_;
    position_ += velocity_;
}
