#include <SDL_events.h>

#include "InputManager.hpp"
#include "Vector3.hpp"

InputManager::InputManager() noexcept
{
}

auto InputManager::player_direction_states() const -> PlayerDirections
{
    SDL_PumpEvents();
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    PlayerDirections player_directions;
    player_directions.Forward = keyboard_state[controls_.WalkForward];
    player_directions.Left = keyboard_state[controls_.WalkLeft];
    player_directions.Backward = keyboard_state[controls_.WalkBackward];
    player_directions.Right = keyboard_state[controls_.WalkRight];
    return player_directions;
}

auto InputManager::player_direction_vector() const -> Vector3d
{
    PlayerDirections player_directions = player_direction_states();
    Vector3d direction_vector;
    direction_vector.z -= player_directions.Forward;
    direction_vector.x -= player_directions.Left;
    direction_vector.z += player_directions.Backward;
    direction_vector.x += player_directions.Right;
    return direction_vector;
}
