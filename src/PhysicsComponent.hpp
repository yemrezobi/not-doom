#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <SDL.h>
#include <Vec3.hpp>

constexpr double kGravitationalConstant = 0.1;

class PhysicsComponent {
    public:
        PhysicsComponent(Vec3 position, Vec3 velocity = Vec3::Zero(), Vec3 acceleration = Vec3::Zero());
        auto GetPosition() const -> Vec3;
        auto SetPosition(const Vec3& position) -> void;
        auto GetVelocity() const -> Vec3;
        auto SetVelocity(const Vec3& velocity) -> void;
        auto SetAffectedByGravity(bool val) -> void;
        auto IsOnGround() -> bool;
        auto Update() -> void;
    private:
        Vec3 position_;
        Vec3 velocity_;
        Vec3 acceleration_;
        bool is_affected_by_gravity_;
};

#endif // PHYSICSCOMPONENT_H
