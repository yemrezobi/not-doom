#include "Doom.hpp"

#include <cstddef>
#include <algorithm>
#include <cmath>
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
    logger_.to_console();

    component_manager_.register_component<TransformComponent>();
    component_manager_.register_component<RenderComponent>();
    component_manager_.register_component<PhysicsComponent>();

    systems_.push_back(std::bind(&Doom::process_physics, this));
    systems_.push_back(std::bind(&Doom::process_renders, this));

    auto& transform_components = component_manager_.get_components<TransformComponent>();
    auto& render_components = component_manager_.get_components<RenderComponent>();
    auto& physics_components = component_manager_.get_components<PhysicsComponent>();

    const Quaterniond direction = Quaterniond::identity();
    // initialize camera
    transform_components.insert_component({player_id_, {0, 0, -200}, direction, Vector3d::identity()});
    physics_components.insert_component({player_id_, {}, {}, false});

    const int e1 = entity_manager_.create_entity();
    transform_components.insert_component({e1, {-50 - 128, 50, 0}, direction, Vector3d::identity()});
    render_components.insert_component(
        {e1, RenderComponent::RenderableType::quad, resource_manager_.get_texture("brick"), 128, 128});

    const int e2 = entity_manager_.create_entity();
    transform_components.insert_component({e2, {50, 50, 0}, direction, Vector3d::identity()});
    render_components.insert_component(
        {e2, RenderComponent::RenderableType::quad, resource_manager_.get_texture("brick1"), 128, 128});

    const int e3 = entity_manager_.create_entity();
    transform_components.insert_component({e3, {-50 - 128, -50 - 128, 0}, direction, Vector3d::identity()});
    render_components.insert_component(
        {e3, RenderComponent::RenderableType::quad, resource_manager_.get_texture("brick2"), 128, 128});

    const int e4 = entity_manager_.create_entity();
    transform_components.insert_component({e4, {50, -50 - 128, 0}, direction, Vector3d::identity()});
    render_components.insert_component(
        {e4, RenderComponent::RenderableType::quad, resource_manager_.get_texture("wood"), 128, 128});

    const int e5 = entity_manager_.create_entity();
    transform_components.insert_component({e5, {-64, -64, 0}, direction, Vector3d::identity()});
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
    camera_angles_.x = std::fmod(camera_angles_.x + event.yrel / full_rotation, 2 * std::numbers::pi);
    camera_angles_.y = std::fmod(camera_angles_.y - event.xrel / full_rotation, 2 * std::numbers::pi);
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
    }
}

auto Doom::process_physics() -> void
{
    auto& transform_vector = component_manager_.get_components<TransformComponent>();
    auto& physics_vector = component_manager_.get_components<PhysicsComponent>();

    // Add movement speed to player's velocity
    auto& player_physics_component = *physics_vector.find_component(player_id_);
    const Vector3d player_movement_vector = input_manager_.player_movement_vector() * player_movement_speed;
    player_physics_component.velocity += player_movement_vector;

    for (auto& physics_component : physics_vector) {
        const Vector3d delta_v = physics_component.acceleration * delta_time_;
        auto& transform_component = *transform_vector.find_component(physics_component.entity_id);
        physics_component.velocity += delta_v;
        transform_component.position += delta_v * delta_time_ / 2;
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

    for (auto& render_component : render_vector) {
        // TODO: handle scale
        auto& transform_component = *transform_vector.find_component(render_component.entity_id);
        switch (render_component.type) {
            case RenderComponent::RenderableType::sprite: {
                const Vector3d transformed_pos = camera_transform.rotation.inverse().rotate_point(transform_component.position) - camera_transform.position;
                const SDL_FRect rect{static_cast<float>(transformed_pos.x + k_window_width / 2),
                    static_cast<float>(transformed_pos.y + k_window_height / 2),
                    static_cast<float>(render_component.width), static_cast<float>(render_component.height)};
                SDL_RenderCopyF(renderer_, render_component.texture, NULL, &rect);
                break;
            }
            case RenderComponent::RenderableType::quad: {
                const Quaterniond total_rotation = camera_transform.rotation * transform_component.rotation;
                const Vector3d translated_position = transform_component.position - camera_transform.position;
                // top left
                const Vector3d vec1 = total_rotation.rotate_point(translated_position);
                // top right
                const Vector3d vec2 = total_rotation.rotate_point(translated_position + Vector3d{render_component.width, 0, 0});
                // bot left
                const Vector3d vec3 = total_rotation.rotate_point(translated_position + Vector3d{0, render_component.height, 0});
                // bot right
                const Vector3d vec4 = total_rotation.rotate_point(translated_position + Vector3d{render_component.width, render_component.height, 0});
                // TODO: z culling
                if (vec1.z < 0 || vec2.z < 0 || vec3.z < 0 || vec4.z <0) {
                    break;
                }
                const Vector2d v1{vec1.x + k_window_width / 2, vec1.y + k_window_height / 2};
                const Vector2d v2{vec2.x + k_window_width / 2, vec2.y + k_window_height / 2};
                const Vector2d v3{vec3.x + k_window_width / 2, vec3.y + k_window_height / 2};
                const Vector2d v4{vec4.x + k_window_width / 2, vec4.y + k_window_height / 2};
                const double aspect_ratio = k_window_width / k_window_height;
                const double tesselation_tile_height = k_window_height / 50;
                const double tesselation_tile_width = tesselation_tile_height * aspect_ratio;
                const int tesselation_width_factor =
                    std::floor(std::max(std::fabs(v2.x - v1.x), fabs(v4.x - v3.x)) / tesselation_tile_width);
                const int tesselation_height_factor =
                    std::floor(std::max(std::fabs(v3.y - v1.y), fabs(v4.y - v2.y)) / tesselation_tile_height);
                const auto tesselated_quad =
                    tesselate(v1, v2, v3, v4, tesselation_height_factor, tesselation_width_factor);
                SDL_RenderGeometry(renderer_, render_component.texture, tesselated_quad.vertices.get(),
                    tesselated_quad.vertices_length, tesselated_quad.indices.get(), tesselated_quad.indices_length);
                break;
            }
        }
    }

    // TODO: delet this
    // draw makeshift crosshairs
    // if (SDL_SetRenderDrawColor(renderer_, 0, 0, 255, SDL_ALPHA_OPAQUE)) {
    //    logger_.error(std::string("Could not set render draw color: ").append(SDL_GetError()));
    //}
    // SDL_RenderDrawLine(renderer_, static_cast<int>(k_window_width / 2) - 50, static_cast<int>(k_window_height / 2),
    //    static_cast<int>(k_window_width / 2) + 50, static_cast<int>(k_window_height / 2));
    // SDL_RenderDrawLine(renderer_, static_cast<int>(k_window_width / 2), static_cast<int>(k_window_height / 2) - 50,
    //    static_cast<int>(k_window_width / 2), static_cast<int>(k_window_height / 2) + 50);

    SDL_RenderPresent(renderer_);
}

auto Doom::tesselate(const Vector2d v1, const Vector2d v2, const Vector2d v3, const Vector2d v4,
    const int height_factor, const int width_factor) const -> const TesselationResult
{
    // populate vertices
    std::unique_ptr<SDL_Vertex[]> vertices = std::make_unique<SDL_Vertex[]>((height_factor + 1) * (width_factor + 1));
    for (int i = 0; i < height_factor + 1; i++) {
        const double left_pivot_x = v1.x + ((v3.x - v1.x) / height_factor * i);
        const double left_pivot_y = v1.y + ((v3.y - v1.y) / height_factor * i);
        const double right_pivot_x = v2.x + ((v4.x - v2.x) / height_factor * i);
        const double right_pivot_y = v2.y + ((v4.y - v2.y) / height_factor * i);
        for (int j = 0; j < width_factor + 1; j++) {
            SDL_Vertex& vertex = vertices[i * (width_factor + 1) + j];
            vertex.position = {
                static_cast<float>(left_pivot_x + (std::fabs(right_pivot_x - left_pivot_x) / width_factor * j)),
                static_cast<float>(left_pivot_y + (std::fabs(right_pivot_y - left_pivot_y) / width_factor * j))};
            vertex.color = {255, 255, 255, 0};
            vertex.tex_coord = {
                static_cast<float>(1.0 / width_factor * j), static_cast<float>(1.0 / height_factor * i)};
        }
    }

    // populate indices
    std::unique_ptr<int[]> indices = std::make_unique<int[]>(height_factor * width_factor * 6);
    for (int i = 0; i < height_factor; i++) {
        for (int j = 0; j < width_factor; j++) {
            const int indices_index = (i * width_factor + j) * 6;
            const int vertices_index = i * (width_factor + 1) + j;
            indices[indices_index] = vertices_index;
            indices[indices_index + 1] = vertices_index + 1;
            indices[indices_index + 2] = vertices_index + width_factor + 1;
            indices[indices_index + 3] = vertices_index + 1;
            indices[indices_index + 4] = vertices_index + width_factor + 2;
            indices[indices_index + 5] = vertices_index + width_factor + 1;
            indices[indices_index + 5] = vertices_index + width_factor + 1;
        }
    }
    return {std::move(vertices), (height_factor + 1) * (width_factor + 1), std::move(indices),
        height_factor * width_factor * 6};
}
