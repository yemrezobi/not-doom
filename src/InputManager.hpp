#pragma once

#include <unordered_map>

#include <SDL_scancode.h>

#include "Vector3.hpp"

class InputManager {
public:
    // 1 for true, 0 for false
    // makes calculating direction vector easier
    struct PlayerDirections {
        int forward;
        int left;
        int backward;
        int right;
    };

    struct KeyState {
        // whether the key is currently pressed
        bool is_pressed = false;
        // whether the key became pressed this frame
        bool key_down = false;
        // whether the key became released this frame
        bool key_up = false;
    };

    // clang-format off
    struct {
        SDL_Scancode walk_forward = SDL_SCANCODE_W;
        SDL_Scancode walk_left = SDL_SCANCODE_A;
        SDL_Scancode walk_backward = SDL_SCANCODE_S;
        SDL_Scancode walk_right = SDL_SCANCODE_D;
        SDL_Scancode jump = SDL_SCANCODE_SPACE;
        SDL_Scancode crouch = SDL_SCANCODE_LCTRL;
        SDL_Scancode exit_game = SDL_SCANCODE_ESCAPE;
        SDL_Scancode reset_camera = SDL_SCANCODE_GRAVE;
    } controls;
    // clang-format on

    auto get_key_state(SDL_Scancode key_code) -> KeyState&;
    auto set_key_state(SDL_Scancode key_code, bool state) -> void;
    auto get_player_movement_vector() -> const Vector3d;

private:
    std::unordered_map<SDL_Scancode, KeyState> key_states_;
    auto poll_player_movement_states() -> const PlayerDirections;
};
