#include <SDL_events.h>

#include "InputManager.hpp"
#include "Vector3.hpp"

auto InputManager::get_key_state(SDL_Scancode key_code) -> KeyState&
{
    return key_states_[key_code];
}

auto InputManager::set_key_state(SDL_Scancode key_code, bool state) -> void
{
    KeyState& key_state = get_key_state(key_code);
    key_state.key_down = false;
    key_state.key_up = false;
    key_state.key_down = state && !key_state.is_pressed;
    key_state.key_up = !state && key_state.is_pressed;
    key_state.is_pressed = state;
}

auto InputManager::player_movement_vector() -> const Vector3d
{
    PlayerDirections player_directions = player_movement_states();
    Vector3d direction_vector;
    direction_vector.z -= player_directions.forward;
    direction_vector.x -= player_directions.left;
    direction_vector.z += player_directions.backward;
    direction_vector.x += player_directions.right;
    return direction_vector;
}

auto InputManager::player_movement_states() -> const PlayerDirections
{
    PlayerDirections player_directions;
    player_directions.forward = get_key_state(InputManager::controls.walk_forward).is_pressed;
    player_directions.left = get_key_state(InputManager::controls.walk_left).is_pressed;
    player_directions.backward = get_key_state(InputManager::controls.walk_backward).is_pressed;
    player_directions.right = get_key_state(InputManager::controls.walk_right).is_pressed;
    return player_directions;
}
