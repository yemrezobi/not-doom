#include <PhysicsComponent.hpp>
#include <Player.hpp>
#include <Debug.hpp>

Player::Player(double x, double y, double z)
    : physics_(Vec3(x, y, z)),
      walking_direction_(0u)
{
    physics_.SetAffectedByGravity(true);
}

auto Player::GetPhysicsRef() -> PhysicsComponent&
{
    return physics_;
}

auto Player::GetPhysics() const -> PhysicsComponent
{
    return physics_;
}

auto Player::SetPhysics(const PhysicsComponent& physics) -> void
{
    physics_ = physics;
}

auto Player::SetWalkingDirection(WalkingDirection direction, bool value) -> void
{
    if (value == static_cast<bool>(walking_direction_ & direction)) {
        // if already (not) going this direction
        return;
    }
    if (value) {
        walking_direction_ |= direction;
    } else {
        walking_direction_ &= ~direction;
    }
    double mod = value ? 1.0 : -1.0; // add or remove velocity

    Vec3 velocity = physics_.GetVelocity();
    if(walking_direction_ & WalkingDirection::kForward){
        velocity.z += -1.0 * mod;
    }
    if(walking_direction_ & WalkingDirection::kLeft){
        velocity.x += -1.0 * mod;
    }
    if(walking_direction_ & WalkingDirection::kBackward){
        velocity.z += 1.0 * mod;
    }
    if(walking_direction_ & WalkingDirection::kRight){
        velocity.x += 1.0 * mod;
    }
    physics_.SetVelocity(velocity);
}

auto Player::Jump() -> void
{
    if (!physics_.IsOnGround()) {
        return;
    }
    Vec3 velocity = physics_.GetVelocity();
    velocity.y += kJumpVelocity;
    physics_.SetVelocity(velocity);
}

auto Player::Update() -> void
{
    



    auto pos = physics_.GetPosition();
    std::cout << std::format("x: {}, y: {}, z: {}\n", pos.x, pos.y, pos.z);
    physics_.Update();
}
