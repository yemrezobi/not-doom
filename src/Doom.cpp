#include "Doom.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <numbers>
#include <string>
#include <utility>
#include <vector>

#include <SDL_error.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_video.h>

#include "ComponentManager.hpp"
#include "ComponentVector.hpp"
#include "EntityManager.hpp"
#include "InputManager.hpp"
#include "LoggingManager.hpp"
#include "Matrix4x4.hpp"
#include "PhysicsComponent.hpp"
#include "Quaternion.hpp"
#include "RenderComponent.hpp"
#include "ResourceManager.hpp"
#include "TransformComponent.hpp"
#include "Vector3.hpp"

Doom::Doom() : player_movement_speed{1.0}, logger_{LoggingManager::LogLevel::debug}
{
    player_id_ = entity_manager_.create_entity();
    camera_angles_ = {0, 0};
}

auto Doom::setup() -> void
{
    if (!logger_.to_console()) {
        logger_.to_file(runtime_dir_ / "doom.log");
        logger_.warning("Could not initialize console logging");
    }
    logger_.info("Started logging");

    component_manager_.register_component<TransformComponent>();
    component_manager_.register_component<RenderComponent>();
    component_manager_.register_component<PhysicsComponent>();

    systems_.push_back(std::bind(&Doom::process_physics, this));
    systems_.push_back(std::bind(&Doom::process_renders, this));

    auto& transform_components = component_manager_.get_components<TransformComponent>();
    auto& render_components = component_manager_.get_components<RenderComponent>();
    auto& physics_components = component_manager_.get_components<PhysicsComponent>();

    const Quaterniond direction = Quaterniond::identity();
    const Vector3d scale = {1, 1, 1};
    // initialize camera
    transform_components.insert_component({player_id_, {0, 0, 0}, direction, scale});
    physics_components.insert_component({player_id_, {}, {}, false});

    const int e1 = entity_manager_.create_entity();
    transform_components.insert_component({e1, {-50 - 128, 50, 20}, direction, scale});
    render_components.insert_component(
        {e1, RenderComponent::RenderableType::quad, resource_manager_.get_texture("brick"), 128, 128});

    const int e2 = entity_manager_.create_entity();
    transform_components.insert_component({e2, {50, 50, 20}, direction, scale});
    render_components.insert_component(
        {e2, RenderComponent::RenderableType::quad, resource_manager_.get_texture("brick1"), 128, 128});

    const int e3 = entity_manager_.create_entity();
    transform_components.insert_component({e3, {-50 - 128, -50 - 128, 20}, direction, scale});
    render_components.insert_component(
        {e3, RenderComponent::RenderableType::quad, resource_manager_.get_texture("brick2"), 128, 128});

    const int e4 = entity_manager_.create_entity();
    transform_components.insert_component({e4, {50, -50 - 128, 20}, direction, scale});
    render_components.insert_component(
        {e4, RenderComponent::RenderableType::quad, resource_manager_.get_texture("wood"), 128, 128});

    const int e5 = entity_manager_.create_entity();
    transform_components.insert_component({e5, {-64, -64, 20}, direction, scale});
    render_components.insert_component(
        {e5, RenderComponent::RenderableType::quad, resource_manager_.get_texture("aqpanl03"), 128, 128});
}

auto Doom::handle_event_window(const SDL_WindowEvent& event) -> void
{
    switch (event.event) {
        case SDL_WINDOWEVENT_CLOSE: {
            logger_.info("Window closing");
            exiting_ = true;
            break;
        }
        default: {
            break;
        }
    }
}

auto Doom::handle_event_mouse_motion(const SDL_MouseMotionEvent& event) -> void
{
    // how many mouse movement units a full rotation takes
    constexpr double full_rotation = 50 * 2 * std::numbers::pi;
    // the mouse rotation around axes in radians
    camera_angles_.x = std::fmod(camera_angles_.x - event.yrel / full_rotation, 2 * std::numbers::pi);
    camera_angles_.y = std::fmod(camera_angles_.y + event.xrel / full_rotation, 2 * std::numbers::pi);
    auto& player_transform = *component_manager_.get_components<TransformComponent>().find_component(player_id_);
    player_transform.rotation = Quaterniond::from_axis_angle({0, 1, 0}, camera_angles_.y)
        * Quaterniond::from_axis_angle({1, 0, 0}, camera_angles_.x);
}

auto Doom::handle_event_key(const SDL_KeyboardEvent& event) -> void
{
    const SDL_Scancode key_code = event.keysym.scancode;
    input_manager_.set_key_state(key_code, event.state);
    const InputManager::KeyState key_state = input_manager_.get_key_state(event.keysym.scancode);
    if (key_code == input_manager_.controls.exit_game && key_state.key_down) {
        exiting_ = true;
    } else if (key_code == input_manager_.controls.reset_camera && key_state.key_down) {
        component_manager_.get_components<TransformComponent>().find_component(player_id_)->rotation = Quaterniond::identity();
        camera_angles_ = {0, 0};
    }
}

auto Doom::process_physics() -> void
{
    auto& transform_vector = component_manager_.get_components<TransformComponent>();
    auto& physics_vector = component_manager_.get_components<PhysicsComponent>();

    // Add movement speed to player's velocity
    auto& player_physics_component = *physics_vector.find_component(player_id_);
    const Vector3d player_movement_vector = input_manager_.get_player_movement_vector() * player_movement_speed;
    player_physics_component.velocity += player_movement_vector;

    for (auto& physics_component : physics_vector) {
        auto& transform_component = *transform_vector.find_component(physics_component.entity_id);
        const Vector3d delta_v = physics_component.acceleration * delta_time_;
        transform_component.position += physics_component.velocity * delta_time_;
        transform_component.position += delta_v * delta_time_ / 2;
        physics_component.velocity += delta_v;
        // TODO: handle collision
        transform_component.position.y -= PhysicsComponent::k_gravity * physics_component.is_affected_by_gravity;
    }

    // Remove movement speed from player's velocity
    player_physics_component.velocity -= player_movement_vector;
}

auto Doom::process_renders() -> void
{
    auto& transform_vector = component_manager_.get_components<TransformComponent>();
    auto& render_vector = component_manager_.get_components<RenderComponent>();
    auto& camera_transform = *transform_vector.find_component(player_id_);

    // RenderComponents need to be drawn in order of their z
    std::sort(render_vector.begin(), render_vector.end(), [&](RenderComponent lhs, RenderComponent rhs) {
        return transform_vector.find_component(lhs.entity_id) < transform_vector.find_component(rhs.entity_id);
    });

    if (SDL_SetRenderDrawColor(renderer_, 255, 0, 0, SDL_ALPHA_OPAQUE)) {
        logger_.error(std::string("Could not set render draw color: ").append(SDL_GetError()));
    }

    if (SDL_RenderClear(renderer_)) {
        logger_.error(std::string("Could not clear render target: ").append(SDL_GetError()));
    }

    constexpr double near = 1.0;
    constexpr double far = 20.0;
    const double fov = 70.0;
    const Matrix4x4d perspective_matrix =
        Matrix4x4d::perspective_matrix(near, far, k_window_width / k_window_height, fov);
    for (auto& render_component : render_vector) {
        auto& transform_component = *transform_vector.find_component(render_component.entity_id);
        switch (render_component.type) {
            case RenderComponent::RenderableType::sprite: {
                const Vector3d transformed_pos =
                    camera_transform.rotation.inverse().rotate_point(
                        transform_component.position.element_wise(transform_component.scale))
                    - camera_transform.position;
                const SDL_FRect rect{static_cast<float>(transformed_pos.x + k_window_width / 2),
                    static_cast<float>(transformed_pos.y + k_window_height / 2),
                    static_cast<float>(render_component.width), static_cast<float>(render_component.height)};
                SDL_RenderCopyF(renderer_, render_component.texture, NULL, &rect);
                break;
            }
            case RenderComponent::RenderableType::quad: {
                const Matrix4x4d total_rotation = perspective_matrix
                    * Matrix4x4d::from_quaternion(camera_transform.rotation * transform_component.rotation);
                const Vector3d translated_position =
                    transform_component.position.element_wise(transform_component.scale) - camera_transform.position;
                // top left
                const Vector3d vec1 = total_rotation.rotate_point(translated_position);
                // top right
                const Vector3d vec2 =
                    total_rotation.rotate_point(translated_position + Vector3d{render_component.width, 0, 0});
                // bot left
                const Vector3d vec3 =
                    total_rotation.rotate_point(translated_position + Vector3d{0, render_component.height, 0});
                // bot right
                const Vector3d vec4 = total_rotation.rotate_point(
                    translated_position + Vector3d{render_component.width, render_component.height, 0});
                // TODO: z culling
                if (vec1.z < near || vec2.z < near || vec3.z < near || vec4.z < near) {
                    break;
                }
                if (vec1.z > far || vec2.z > far || vec3.z > far || vec4.z > far) {
                    break;
                }
                const Vector2d v1{vec1.x + k_window_width / 2, vec1.y + k_window_height / 2};
                const Vector2d v2{vec2.x + k_window_width / 2, vec2.y + k_window_height / 2};
                const Vector2d v3{vec3.x + k_window_width / 2, vec3.y + k_window_height / 2};
                const Vector2d v4{vec4.x + k_window_width / 2, vec4.y + k_window_height / 2};
                const SDL_Vertex vx1{{static_cast<float>(v1.x), static_cast<float>(v1.y)}, {255, 255, 255, 0}, {0, 0}};
                const SDL_Vertex vx2{{static_cast<float>(v2.x), static_cast<float>(v2.y)}, {255, 255, 255, 0}, {1, 0}};
                const SDL_Vertex vx3{{static_cast<float>(v3.x), static_cast<float>(v3.y)}, {255, 255, 255, 0}, {0, 1}};
                const SDL_Vertex vx4{{static_cast<float>(v4.x), static_cast<float>(v4.y)}, {255, 255, 255, 0}, {1, 1}};
                SDL_Vertex vertices[4]{vx1, vx2, vx3, vx4};
                int indices[6]{0, 1, 2, 1, 3, 2};
                SDL_RenderGeometry(renderer_, render_component.texture, vertices,
                    4, indices, 6);
                break;
            }
        }
    }

    SDL_RenderPresent(renderer_);
}
