#pragma once

#include <SDL_events.h>
#include <functional>
#include <stdint.h>
#include <vector>

#include <SDL_render.h>
#include <SDL_video.h>

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "InputManager.hpp"
#include "LoggingManager.hpp"
#include "ResourceManager.hpp"

class Game {
public:
    Game();
    ~Game();
    auto start() -> void;
    const double k_window_width;
    const double k_window_height;

protected:
    bool exiting_;
    SDL_Renderer* renderer_;
    EntityManager entity_manager_;
    ComponentManager component_manager_;
    InputManager input_manager_;
    ResourceManager resource_manager_;
    std::vector<std::function<void()>> systems_;
    double delta_time_;
    std::filesystem::path runtime_dir_;

    auto virtual handle_event_window(const SDL_WindowEvent&) -> void
    {
    }

    auto virtual handle_event_mouse_motion(const SDL_MouseMotionEvent&) -> void
    {
    }

    auto virtual handle_event_key(const SDL_KeyboardEvent&) -> void
    {
    }

private:
    LoggingManager logger_;
    SDL_Window* window_;
    Uint64 prev_time_;

    virtual auto setup() -> void = 0;
    auto loop() -> void;
};
