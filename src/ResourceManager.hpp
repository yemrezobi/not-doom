#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include <SDL_render.h>

class LoggingManager;

class ResourceManager {
public:
    explicit ResourceManager(const std::filesystem::path& asset_directory);
    explicit ResourceManager();
    ~ResourceManager();

    std::filesystem::path asset_directory;

    auto set_context(SDL_GLContext context) -> void;
    auto get_texture(std::string texture_name) -> SDL_Texture*;

private:
    SDL_GLContext context_;
    std::unordered_map<std::string, SDL_Texture*> textures_;
};
