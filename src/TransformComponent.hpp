#pragma once

#include "Vector3.hpp"
#include "Quaternion.hpp"

struct TransformComponent {
    int entity_id;
    Vector3d position;
    Quaterniond rotation;
    Vector3d scale;
};
