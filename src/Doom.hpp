#pragma once

#include <array>

#include <GL/glew.h>
#include <SDL_events.h>
#include <SDL_render.h>

#include "Game.hpp"
#include "Logger.hpp"
#include "Mesh.hpp"

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
    GLuint shader_program_;
    GLuint vao_;
    GLuint vbo_;
    GLuint cbo_;
    GLuint ibo_;
    std::array<float, 16> perspective_matrix_;

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
