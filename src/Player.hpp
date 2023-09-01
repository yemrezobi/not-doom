#ifndef PLAYER_H
#define PLAYER_H

#include <PhysicsComponent.hpp>
#include <Camera.hpp>

class Player {
    public:
        Player(double x, double y, double z);
        enum WalkingDirection : uint8_t {
            kForward  = (1u << 0),
            kLeft     = (1u << 1),
            kBackward = (1u << 2),
            kRight    = (1u << 3),
        };
        static constexpr double kJumpVelocity = 10.0;
        auto GetPhysicsRef() -> PhysicsComponent&;
        auto GetPhysics() const -> PhysicsComponent;
        auto SetPhysics(const PhysicsComponent& physics_) -> void;
        auto SetWalkingDirection(WalkingDirection direction, bool value) -> void;
        auto Jump() -> void;
        auto Update() -> void;
    private:
        PhysicsComponent physics_;
        uint8_t walking_direction_;
        uint8_t prev_walking_direction_;
};

#endif // PLAYER_H
