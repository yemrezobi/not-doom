#pragma once

#include "Vector3.hpp"

struct PhysicsComponent {
    static constexpr float k_gravity = 0.1f;
    int entity_id;
    Vector3f velocity;
    Vector3f acceleration;
    bool is_affected_by_gravity;
};
