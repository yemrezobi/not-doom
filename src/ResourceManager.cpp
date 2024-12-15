#include "ResourceManager.hpp"

#include <cstddef>
#include <filesystem>
#include <utility>

#include <SDL_error.h>
#include <SDL_image.h>

#include "Logger.hpp"

ResourceManager::ResourceManager(const std::filesystem::path& asset_directory) : asset_directory{asset_directory}, context_{nullptr}, textures_{}
{
}

ResourceManager::ResourceManager() : asset_directory{}, context_{nullptr}, textures_{}
{
}

ResourceManager::~ResourceManager()
{
    for (auto& texture : textures_) {
        SDL_DestroyTexture(texture.second);
    }
}

auto ResourceManager::set_context(SDL_GLContext context) -> void
{
    context_ = context;
}

auto ResourceManager::get_texture(std::string texture_name) -> SDL_Texture*
{
    /*
    if (!textures_.contains(texture_name)) [[unlikely]] {
        const std::filesystem::path texture_path = asset_directory / "patches" / (texture_name + ".png");
        textures_[texture_name] = IMG_LoadTexture(renderer_, texture_path.string().c_str());
        if (textures_[texture_name] == NULL) {
            Logger::get_default()->error(std::format("Could not load texture: {}", SDL_GetError());
        }
    }
    return textures_[texture_name];
    */
    (void)texture_name;
    return nullptr;
}
