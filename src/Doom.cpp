#include "Doom.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iterator>
#include <numbers>
#include <numeric>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <GL/glew.h>
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
#include "Logger.hpp"
#include "Matrix4x4.hpp"
#include "Mesh.hpp"
#include "MeshComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Quaternion.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include "TransformComponent.hpp"
#include "Vector3.hpp"

Doom::Doom() :
    player_movement_speed{0.05f}, vao_{NULL} , vbo_{NULL}, cbo_{NULL}, ibo_{NULL}, shader_program_{NULL}, perspective_matrix_()
{
    player_id_ = entity_manager_.create_entity();
    camera_angles_ = {0.0f, 0.0f};
}

auto Doom::setup() -> void
{
    //////////////////// OpenGL ////////////////////

    // create buffers
    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glCreateBuffers(1, &vbo_);
    glCreateBuffers(1, &cbo_);
    glCreateBuffers(1, &ibo_);
    glVertexArrayElementBuffer(vao_, ibo_);

    // construct shaders
    std::vector<GLuint> shader_list;

    constexpr std::string_view vertex_shader_string = R"(
#version 460 core

#define PI 3.1415926538f
#define MAX_INSTANCE_COUNT 100

layout (location = 0) in vec4 vector_position;
layout (location = 1) in vec4 color;
uniform mat4 model_views[MAX_INSTANCE_COUNT];
uniform mat4 projection;
smooth out vec4 pass_color;

void main()
{
    gl_Position = projection * model_views[gl_InstanceID + gl_BaseInstance] * vector_position;
    pass_color = color;
}
    )";
    std::optional<GLuint> vertex_shader = shader::create_shader(vertex_shader_string, GL_VERTEX_SHADER);
    if (!vertex_shader) [[unlikely]] {
        fatal_error("Error compiling vertex shader.");
    } else [[likely]] {
        shader_list.push_back(*vertex_shader);
    }

    constexpr std::string_view fragment_shader_string = R"(
#version 460 core

smooth in vec4 pass_color;
out vec4 out_color;

void main()
{
    out_color = pass_color;
}
    )";
    std::optional<GLuint> fragment_shader = shader::create_shader(fragment_shader_string, GL_FRAGMENT_SHADER);
    if (!fragment_shader) [[unlikely]] {
        fatal_error("Error compiling fragment shader.");
    } else [[likely]] {
        shader_list.push_back(*fragment_shader);
    }

    std::optional<GLuint> shader_program = shader::create_shader_program(shader_list);
    if (!shader_program) [[unlikely]] {
        fatal_error("Error compiling shader program.");
    }
    else [[likely]] {
        shader_program_ = *shader_program;
    }
    std::for_each(shader_list.begin(), shader_list.end(), glDeleteShader);

    // create perspective matrix
    perspective_matrix_.fill(0.0f);
    constexpr float near = 0.5f;
    constexpr float far = 500.f;
    constexpr float fov = 75.0f; //vertical fov in degrees
    const float aspect_ratio = k_window_width / k_window_height;
    const float inv_tan = 1 / std::tan(fov * (std::numbers::pi / 180.0f) / 2.0f);
    perspective_matrix_[0] = inv_tan / aspect_ratio;
    perspective_matrix_[5] = inv_tan;
    perspective_matrix_[10] = (near + far) / (near - far);
    perspective_matrix_[14] = (2 * near * far) / (near - far);
    perspective_matrix_[11] = -1.0f;

    glProgramUniformMatrix4fv(shader_program_, glGetUniformLocation(shader_program_, "projection"), 1, GL_FALSE, perspective_matrix_.data());

    // bind buffers and specify format
    constexpr uint32_t vertex_buffer_bind_index{0};
    constexpr size_t position_vector_size{3 * sizeof(float)};
    glVertexArrayVertexBuffer(vao_, vertex_buffer_bind_index, vbo_, 0, position_vector_size);
    glVertexArrayAttribBinding(vao_, 0, vertex_buffer_bind_index);
    glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, 0);

    constexpr uint32_t color_buffer_bind_index{1};
    constexpr size_t color_vector_size{4 * sizeof(float)};
    glVertexArrayVertexBuffer(vao_, color_buffer_bind_index, cbo_, 0, color_vector_size);
    glVertexArrayAttribBinding(vao_, 1, color_buffer_bind_index);
    glVertexArrayAttribFormat(vao_, 1, 4, GL_FLOAT, GL_FALSE, 0);
    //TODO: is this needed?
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    component_manager_.register_component<TransformComponent>();
    component_manager_.register_component<MeshComponent>();
    component_manager_.register_component<PhysicsComponent>();

    systems_.push_back(std::bind(&Doom::process_physics, this));
    systems_.push_back(std::bind(&Doom::process_renders, this));

    auto& transform_components = component_manager_.get_components<TransformComponent>();
    auto& mesh_components = component_manager_.get_components<MeshComponent>();
    auto& physics_components = component_manager_.get_components<PhysicsComponent>();

    const TransformComponent player_transform
    {
        .entity_id = player_id_,
        .position = {0.0f, 0.0f, 5.0f},
        .rotation = Quaternionf::identity(),
        .scale = {1.0f, 1.0f, 1.0f},
    };
    transform_components.insert_component(player_transform);
    physics_components.insert_component({
        .entity_id = player_id_,
        .velocity = {},
        .acceleration = {},
        .is_affected_by_gravity = false,
    });

    TransformComponent origin_cube_transform = {
        .entity_id = entity_manager_.create_entity(),
        .position = {0.0f, 0.0f, 0.0f},
        .rotation = Quaternionf::identity(),
        .scale = {0.2f, 0.2f, 0.2f},
    };
    TransformComponent cube_transform1 = {
        .entity_id = entity_manager_.create_entity(),
        .position = {2.0f, 2.0f, 0.0f},
        .rotation = Quaternionf::identity(),
        .scale = {1.0f, 1.0f, 1.0f},
    };
    TransformComponent cube_transform2 = {
        .entity_id = entity_manager_.create_entity(),
        .position = {-2.0f, 2.0f, 0.0f},
        .rotation = Quaternionf::identity(),
        .scale = {1.0f, 1.0f, 1.0f},
    };
    TransformComponent pyramid_transform1 = {
        .entity_id = entity_manager_.create_entity(),
        .position = {-1.0f, -1.0f, 0.0f},
        .rotation = Quaternionf::identity(),
        .scale = {1.0f, 1.0f, 1.0f},
    };
    TransformComponent pyramid_transform2 = {
        .entity_id = entity_manager_.create_entity(),
        .position = {1.0f, -1.0f, 0.0f},
        .rotation = Quaternionf::identity(),
        .scale = {1.0f, 1.0f, 1.0f},
    };

    transform_components.insert_component(origin_cube_transform);
    mesh_components.insert_component({
        .entity_id = origin_cube_transform.entity_id,
        .mesh_name = "cube",
    });
    transform_components.insert_component(cube_transform1);
    mesh_components.insert_component({
        .entity_id = cube_transform1.entity_id,
        .mesh_name = "cube",
        });
    transform_components.insert_component(cube_transform2);
    mesh_components.insert_component({
        .entity_id = cube_transform2.entity_id,
        .mesh_name = "cube",
    });
    transform_components.insert_component(pyramid_transform1);
    mesh_components.insert_component({
        .entity_id = pyramid_transform1.entity_id,
        .mesh_name = "pyramid",
    });
    transform_components.insert_component(pyramid_transform2);
    mesh_components.insert_component({
        .entity_id = pyramid_transform2.entity_id,
        .mesh_name = "pyramid",
    });

    /*
    const int e1 = entity_manager_.create_entity();
    transform_components.insert_component({e1, {-50 - 128, 50, -20}, direction, scale});
    render_components.insert_component(
        {e1, RenderComponent::RenderableType::quad, resource_manager_.get_texture("brick"), 128, 128});

    const int e2 = entity_manager_.create_entity();
    transform_components.insert_component({e2, {50, 50, -20}, direction, scale});
    render_components.insert_component(
        {e2, RenderComponent::RenderableType::quad, resource_manager_.get_texture("brick1"), 128, 128});

    const int e3 = entity_manager_.create_entity();
    transform_components.insert_component({e3, {-50 - 128, -50 - 128, -20}, direction, scale});
    render_components.insert_component(
        {e3, RenderComponent::RenderableType::quad, resource_manager_.get_texture("brick2"), 128, 128});

    const int e4 = entity_manager_.create_entity();
    transform_components.insert_component({e4, {50, -50 - 128, -20}, direction, scale});
    render_components.insert_component(
        {e4, RenderComponent::RenderableType::quad, resource_manager_.get_texture("wood"), 128, 128});

    const int e5 = entity_manager_.create_entity();
    transform_components.insert_component({e5, {-64, -64, -20}, direction, scale});
    render_components.insert_component(
        {e5, RenderComponent::RenderableType::quad, resource_manager_.get_texture("aqpanl03"), 128, 128});
    */
}

auto Doom::handle_event_window(const SDL_WindowEvent& event) -> void
{
    switch (event.event) {
        case SDL_WINDOWEVENT_CLOSE: {
            Logger::get_default()->info("Window closing");
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
    constexpr float two_pi{2 * std::numbers::pi};
    // how many mouse movement units a full rotation takes
    constexpr float full_rotation = 50 * two_pi;
    // the mouse rotation around axes in radians
    camera_angles_.x = std::fmod(camera_angles_.x - event.xrel / full_rotation, two_pi);
    camera_angles_.y = std::fmod(camera_angles_.y - event.yrel / full_rotation, two_pi);
    auto& player_transform = *component_manager_.get_components<TransformComponent>().find_component(player_id_);
    player_transform.rotation = Quaternionf::from_axis_angle({0, 1, 0}, camera_angles_.x)
        * Quaternionf::from_axis_angle({1, 0, 0}, camera_angles_.y);
}

auto Doom::handle_event_key(const SDL_KeyboardEvent& event) -> void
{
    const SDL_Scancode key_code = event.keysym.scancode;
    input_manager_.update_key_state(key_code, event.state);
    const InputManager::KeyState key_state = input_manager_.get_key_state(event.keysym.scancode);
    if (key_code == input_manager_.controls.exit_game && key_state.key_down) {
        exiting_ = true;
    } else if (key_code == input_manager_.controls.reset_camera && key_state.key_down) {
        component_manager_.get_components<TransformComponent>().find_component(player_id_)->rotation = Quaternionf::identity();
        camera_angles_ = {0, 0};
    }
}

auto Doom::process_physics() -> void
{
    auto& transform_vector = component_manager_.get_components<TransformComponent>();
    auto& physics_vector = component_manager_.get_components<PhysicsComponent>();

    // Add movement speed to player's velocity
    auto& player_physics_component = *physics_vector.find_component(player_id_);
    const Vector3f player_movement_vector = input_manager_.get_player_movement_vector() * player_movement_speed * delta_time_;
    player_physics_component.velocity += player_movement_vector;

    for (auto& physics_component : physics_vector) {
        //TODO: use physics_vector.transform
        auto& transform_component = *transform_vector.find_component(physics_component.entity_id);
        const Vector3f delta_v = physics_component.acceleration * delta_time_;
        transform_component.position += physics_component.velocity * delta_time_;
        transform_component.position += delta_v * delta_time_ / 2;
        physics_component.velocity += delta_v;
        // TODO: handle collision
        transform_component.position.y -= PhysicsComponent::k_gravity * physics_component.is_affected_by_gravity;
    }

    // Remove movement speed from player's velocity
    player_physics_component.velocity -= player_movement_vector;
    auto& player_pos = transform_vector.find_component(player_id_)->position;
}

auto Doom::process_renders() -> void
{
    auto& transform_vector = component_manager_.get_components<TransformComponent>();
    auto& mesh_vector = component_manager_.get_components<MeshComponent>();


    // mapping mesh names to vector of instances of meshes
    // TODO: this can probably be cached somewhere and changed only if a transform/meshcomponent is changed
    std::unordered_map<std::string_view, std::vector<TransformComponent>> mesh_index;
    for (const auto& mesh : mesh_vector) {
        //TODO: linear search in transform_vector for every meshcomponent
        // there should be a better way then O(n*m)
        mesh_index[mesh.mesh_name].push_back(*transform_vector.find_component(mesh.entity_id));
    }

    const size_t vertex_count{std::accumulate(mesh_index.begin(), mesh_index.end(), static_cast<size_t>(0), [](size_t count, const auto& mesh_pair) {
        return count + Mesh::get_mesh(mesh_pair.first)->vertices.size();
        })};

    // data to be uploaded to buffers
    std::vector<float> vertices;
    std::vector<uint16_t> indices;
    std::vector<float> colors;
    vertices.reserve(vertex_count);
    // 4 floats for color for every 3 floats for position
    colors.reserve((vertex_count / 3) * 4);

    int instance_counter{0};
    std::vector<Mesh::DrawElementsIndirectCommand> draw_commands;
    for (const auto& mesh_pair : mesh_index) {
        const Mesh mesh{*Mesh::get_mesh(mesh_pair.first)};
        auto draw_cmd = Mesh::insert_mesh(vertices, colors, indices, mesh);
        const uint32_t instance_count{static_cast<uint32_t>(mesh_pair.second.size())};
        draw_cmd.baseInstance = instance_counter;
        draw_cmd.instanceCount = instance_count;
        draw_commands.push_back(draw_cmd);
        instance_counter += instance_count;
    }

    //TODO: glNamedBufferData reallocates GPU memory every time it is called.
    // Allocating once in setup() with a MAX_VBO_SIZE and using glNamedBufferSubData/glMapNamedBuffer
    // might be more performant.
    glNamedBufferData(vbo_, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(cbo_, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
    glNamedBufferData(ibo_, indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);

    auto camera_transform = *transform_vector.find_component(player_id_);
    camera_transform.position *= -1.0f;
    camera_transform.rotation = camera_transform.rotation.inverse();
    const Matrix4x4f view_matrix = Matrix4x4f::from_transform(camera_transform);

    std::vector<float> model_view_matrices;
    model_view_matrices.resize(instance_counter * 16);
    auto mv_iterator{model_view_matrices.begin()};
    for (const auto& mesh_pair : mesh_index) {
        for (const auto& transform : mesh_pair.second) {
            const Matrix4x4f view_model = view_matrix * Matrix4x4f::from_transform(transform);
            std::copy(view_model.data.begin(), view_model.data.end(), mv_iterator);
            std::advance(mv_iterator, 16);
        }
    }

    glProgramUniformMatrix4fv(shader_program_, glGetUniformLocation(shader_program_, "model_views"), model_view_matrices.size() / 16, GL_TRUE, model_view_matrices.data());

    //TODO: z testing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_);
    glEnableVertexArrayAttrib(vao_, 0);
    glEnableVertexArrayAttrib(vao_, 1);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, draw_commands.data(), draw_commands.size(), 0); 
    glDisableVertexArrayAttrib(vao_, 0);
    glDisableVertexArrayAttrib(vao_, 1);

    glUseProgram(NULL);
}
