#pragma once

#include <memory>

#include <SDL_events.h>
#include <SDL_render.h>

#include "Game.hpp"
#include "LoggingManager.hpp"
#include "Vector2.hpp"

class Doom : public Game {
public:
    Doom();

private:
    int player_id_;
    // euler angles of direction player is looking at
    // to track mouse input
    struct {
        double x;
        double y;
    } camera_angles_;
    double player_movement_speed;
    LoggingManager logger_;
    struct TesselationResult {
        const std::unique_ptr<SDL_Vertex[]> vertices;
        const int vertices_length;
        const std::unique_ptr<int[]> indices;
        const int indices_length;
    };

    auto setup() -> void override;
    auto handle_event_window(const SDL_WindowEvent& event) -> void override;
    auto handle_event_mouse_motion(const SDL_MouseMotionEvent& event) -> void override;
    auto handle_event_key(const SDL_KeyboardEvent& event) -> void override;

    // systems
    // cppcheck-suppress unusedPrivateFunction
    auto process_physics() -> void;
    // cppcheck-suppress unusedPrivateFunction
    auto process_renders() -> void;
};
