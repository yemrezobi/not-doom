#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <functional>
#include <stdint.h>

#include <SDL_render.h>
#include <SDL_video.h>

#include "EntityManager.hpp"
#include "ResourceManager.hpp"
#include "InputManager.hpp"
#include "ComponentManager.hpp"
#include "LoggingManager.hpp"

class Game {
public:
    Game();
    ~Game();
    auto start() -> void;
    const double k_window_width;
    const double k_window_height;

protected:
    SDL_Renderer* renderer_;
    EntityManager entity_manager_;
    ComponentManager component_manager_;
    InputManager input_manager_;
    ResourceManager resource_manager_;
    std::vector<std::function<void()>> systems_;

private:
    LoggingManager logger_;
    SDL_Window* window_;
    bool exiting_;
    uint64_t prev_time_;

    virtual auto setup() -> void = 0;
    auto loop() -> void;
};

#endif // GAME_HPP
