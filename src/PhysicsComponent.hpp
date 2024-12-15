#pragma once

#include "Vector3.hpp"

struct PhysicsComponent {
    static constexpr double k_gravity = 0.1;
    int entity_id;
    Vector3d velocity;
    Vector3d acceleration;
    bool is_affected_by_gravity;
};
