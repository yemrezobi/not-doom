#include <filesystem>

#include <SDL_image.h>

#include "ResourceManager.hpp"

ResourceManager::ResourceManager(const std::filesystem::path& asset_directory) : logger_{nullptr}, asset_directory_{asset_directory}, textures_{}
{
}

ResourceManager::~ResourceManager()
{
    for (auto& texture : textures_) {
        SDL_DestroyTexture(texture.second);
    }
}

auto ResourceManager::set_logger(LoggingManager* logger) -> void
{
    logger_ = logger;
}

auto ResourceManager::set_renderer(SDL_Renderer* renderer) -> void
{
    renderer_ = renderer;
}

auto ResourceManager::get_texture(std::string texture_name) -> SDL_Texture*
{
    if (!textures_.contains(texture_name)) [[unlikely]] {
        const std::filesystem::path texture_path = asset_directory_ / "patches" / (texture_name + ".png");
        textures_[texture_name] = IMG_LoadTexture(renderer_, texture_path.c_str());
        if (textures_[texture_name] == NULL) {
            if (logger_ != nullptr){
                logger_->error(std::string("Could not load texture: ").append(SDL_GetError()));
            }
        }
    }
    return textures_[texture_name];
}
