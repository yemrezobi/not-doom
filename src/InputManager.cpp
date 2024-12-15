#include "InputManager.hpp"

#include "Vector3.hpp"

auto InputManager::get_key_state(SDL_Scancode key_code) -> KeyState&
{
    return key_states_[key_code];
}

auto InputManager::update_key_state(SDL_Scancode key_code, bool state) -> void
{
    KeyState& key_state = get_key_state(key_code);
    key_state.key_down = state && !key_state.is_pressed;
    key_state.key_up = !state && key_state.is_pressed;
    key_state.is_pressed = state;
}

auto InputManager::get_player_movement_vector() -> Vector3f
{
    PlayerDirections player_directions = poll_player_movement_states();
    return{
        static_cast<float>(player_directions.right - player_directions.left),
        0.0f,
        static_cast<float>(player_directions.backward - player_directions.forward)
    };
}

auto InputManager::poll_player_movement_states() -> PlayerDirections
{
    PlayerDirections player_directions;
    player_directions.forward = get_key_state(InputManager::controls.walk_forward).is_pressed;
    player_directions.left = get_key_state(InputManager::controls.walk_left).is_pressed;
    player_directions.backward = get_key_state(InputManager::controls.walk_backward).is_pressed;
    player_directions.right = get_key_state(InputManager::controls.walk_right).is_pressed;
    return player_directions;
}
