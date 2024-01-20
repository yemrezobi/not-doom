#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <string>
#include <unordered_map>
#include <filesystem>

#include <SDL_render.h>

#include "LoggingManager.hpp"

class ResourceManager {
public:
    ResourceManager(const std::filesystem::path& asset_directory);
    ~ResourceManager();


    auto set_logger(LoggingManager* logger) -> void;
    auto set_renderer(SDL_Renderer* renderer) -> void;
    auto get_texture(std::string texture_name) -> SDL_Texture*;

private:
    LoggingManager* logger_;
    std::filesystem::path asset_directory_;
    SDL_Renderer* renderer_;
    std::unordered_map<std::string, SDL_Texture*> textures_;
};

#endif //RESOURCEMANAGER_HPP
