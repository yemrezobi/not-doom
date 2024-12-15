#pragma once

#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

struct TransformComponent {
    int entity_id;
    Vector3f position;
    Quaternionf rotation;
    Vector3f scale;
};
