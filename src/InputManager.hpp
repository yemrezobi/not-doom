#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <SDL_scancode.h>

#include "Vector3.hpp"

class InputManager {
public:
    InputManager() noexcept;
    // 1 for true, 0 for false
    // makes calculating direction vector easier
    struct PlayerDirections {
        int Forward;
        int Left;
        int Backward;
        int Right;
    };
    auto player_direction_vector() const -> Vector3d;
private:
    const struct {
        SDL_Scancode WalkForward = SDL_SCANCODE_W;
        SDL_Scancode WalkLeft = SDL_SCANCODE_A;
        SDL_Scancode WalkBackward = SDL_SCANCODE_S;
        SDL_Scancode WalkRight = SDL_SCANCODE_D;
        SDL_Scancode Jump = SDL_SCANCODE_SPACE;
        SDL_Scancode Crouch = SDL_SCANCODE_LCTRL;
    } controls_;
    auto player_direction_states() const -> PlayerDirections;
};

#endif // INPUTMANAGER_HPP
