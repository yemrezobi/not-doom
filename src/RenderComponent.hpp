#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

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

#endif // RENDERCOMPONENT_HPP
