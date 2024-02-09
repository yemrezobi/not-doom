#ifndef DOOM_HPP
#define DOOM_HPP

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

    // TODO: move this function somefile more relevant
    // take top left, top right, bot left, bot right points respectively
    auto tesselate(const Vector2d v1, const Vector2d v2, const Vector2d v3, const Vector2d v4, const int height_factor, const int width_factor) const -> const TesselationResult;

    // systems
    // cppcheck-suppress unusedPrivateFunction
    auto process_physics() -> void;
    // cppcheck-suppress unusedPrivateFunction
    auto process_renders() -> void;
};

#endif // DOOM_HPP
