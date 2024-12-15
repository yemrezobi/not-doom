#pragma once

#include <functional>
#include <stdint.h>
#include <vector>

#include <GL/glew.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "InputManager.hpp"
#include "Logger.hpp"
#include "ResourceManager.hpp"

class Game {
public:
    Game();
    ~Game();
    auto fatal_error(const std::string& error_msg) -> void;
    auto start() -> void;
    const double k_window_width;
    const double k_window_height;

protected:
    bool exiting_;
    SDL_GLContext context_;
    EntityManager entity_manager_;
    ComponentManager component_manager_;
    InputManager input_manager_;
    ResourceManager resource_manager_;
    std::vector<std::function<void()>> systems_;
    float delta_time_;
    std::filesystem::path runtime_dir_;
    GLuint shader_program_;

    auto get_current_time() const -> float;

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
    SDL_Window* window_;
    uint64_t current_time_;
    uint64_t prev_time_;

    virtual auto setup() -> void = 0;
    auto loop() -> void;
};
