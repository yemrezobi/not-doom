#pragma once

#include "TransformComponent.hpp"

struct MeshComponent {
    int entity_id;
    std::string_view mesh_name;
};
