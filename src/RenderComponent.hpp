#pragma once

#include <SDL_render.h>

struct RenderComponent {
    enum class RenderableType {
        sprite,
        quad
    };

    int entity_id;
    RenderableType type;
    SDL_Texture* texture;
    double width;
    double height;
};
