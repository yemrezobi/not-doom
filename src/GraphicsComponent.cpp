#include <SDL.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <GraphicsComponent.hpp>
#include <SDL2/SDL_image.h>

GraphicsComponent::GraphicsComponent(SDL_Renderer *renderer, const std::string &path) :
    renderer_(renderer),
    texture_(nullptr)
{
    texture_ = IMG_LoadTexture(renderer_, path.c_str());
}

GraphicsComponent::~GraphicsComponent() {
    SDL_DestroyTexture(texture_);
}
