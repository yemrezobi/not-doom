#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

#include <SDL.h>
#include <string>

class GraphicsComponent {
    public:
        GraphicsComponent(SDL_Renderer *renderer, const std::string &path);
        ~GraphicsComponent();
    private:
        SDL_Renderer *renderer_;
        SDL_Texture *texture_;
};

#endif // GRAPHICSCOMPONENT_H
