#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

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

    auto set_logger(LoggingManager* logger) -> void;
    auto set_renderer(SDL_Renderer* renderer) -> void;
    auto get_texture(std::string texture_name) -> SDL_Texture*;

private:
    LoggingManager* logger_;
    SDL_Renderer* renderer_;
    std::unordered_map<std::string, SDL_Texture*> textures_;
};

#endif //RESOURCEMANAGER_HPP
