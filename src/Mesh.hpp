#pragma once

#include <string_view>
#include <unordered_map>

#include <GL/glew.h>

#include "Logger.hpp"

#define COLOR_RED 1.0f, 0.0f, 0.0f, 1.0f
#define COLOR_GREEN 0.0f, 1.0f, 0.0f, 1.0f
#define COLOR_BLUE 0.0f, 0.0f, 1.0f, 1.0f
#define COLOR_YELLOW 1.0f, 1.0f, 0.0f, 1.0f
#define COLOR_CYAN 0.0f, 1.0f, 1.0f, 1.0f
#define COLOR_PURPLE 1.0f, 0.0f, 1.0f, 1.0f

class Mesh {
public:
    typedef struct {
        uint32_t count;
        uint32_t instanceCount;
        uint32_t firstIndex;
        int32_t baseVertex;
        uint32_t baseInstance;
    } DrawElementsIndirectCommand;

    std::vector<float> vertices;
    std::vector<float> colors;
    std::vector<uint16_t> indices;

    auto static insert_mesh(std::vector<float>& buffer, std::vector<float>& color_buffer, std::vector<uint16_t>& indices, const Mesh& mesh) -> DrawElementsIndirectCommand
    {
        const int32_t vertices_start = buffer.size() / 3; //3 floats per vertex
        buffer.insert(buffer.end(), mesh.vertices.begin(), mesh.vertices.end());
        color_buffer.insert(color_buffer.end(), mesh.colors.begin(), mesh.colors.end());
        const uint32_t indices_start = indices.size();
        indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
        return {
            .count = static_cast<uint32_t>(mesh.indices.size()),
            .instanceCount = 0,
            .firstIndex = indices_start,
            .baseVertex = vertices_start,
            .baseInstance = 0,
        };
    }

    auto static get_mesh(std::string_view mesh_name) -> std::optional<Mesh>
    {
        auto mesh_it = mesh_index_.find(mesh_name);
        if (mesh_it == mesh_index_.end()) [[unlikely]] {
            const auto mesh = load_mesh(mesh_name);
            if (mesh == false) [[unlikely]] {
                return std::nullopt;
            }
            mesh_it = mesh_index_.find(mesh_name);
        }
        return mesh_it->second;
    }

private:
    inline static std::unordered_map<std::string_view, Mesh> mesh_index_;

    auto static load_mesh(std::string_view mesh_name) -> bool
    {
        if (mesh_name == "cube") {
            mesh_index_[mesh_name] = {
                .vertices{ 
                    //front
                    -0.5f,  0.5f,  0.5f,
                    -0.5f, -0.5f,  0.5f,
                     0.5f,  0.5f,  0.5f,
                     0.5f, -0.5f,  0.5f,
                    //back
                    -0.5f,  0.5f, -0.5f,
                     0.5f,  0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                     0.5f, -0.5f, -0.5f,
                    //left
                    -0.5f,  0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f,  0.5f,  0.5f,
                    -0.5f, -0.5f,  0.5f,
                    //right
                     0.5f,  0.5f, -0.5f,
                     0.5f,  0.5f,  0.5f,
                     0.5f, -0.5f, -0.5f,
                     0.5f, -0.5f,  0.5f,
                    //top
                    -0.5f,  0.5f, -0.5f,
                    -0.5f,  0.5f,  0.5f,
                     0.5f,  0.5f, -0.5f,
                     0.5f,  0.5f,  0.5f,
                    //bottom
                    -0.5f, -0.5f, -0.5f,
                     0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f,  0.5f,
                     0.5f, -0.5f,  0.5f,
                },
                .colors{
                    // front
                    COLOR_RED, COLOR_RED, COLOR_RED, COLOR_RED,
                    // back
                    COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN,
                    // left
                    COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE,
                    // right
                    COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW,
                    // top
                    COLOR_CYAN, COLOR_CYAN, COLOR_CYAN, COLOR_CYAN,
                    // bottom
                    COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE
                },
                .indices{
                    // front
                    0, 1, 2, 2, 1, 3,
                    // back
                    4, 5, 6, 7, 6, 5,
                    // left
                    8, 9, 10, 10, 9, 11,
                    // right
                    12, 13, 14, 15, 14, 13,
                    // top
                    16, 17, 18, 18, 17, 19,
                    // bottom
                    20, 21, 22, 23, 22, 21
                },
            };
        }
        else if (mesh_name == "pyramid") {
            mesh_index_[mesh_name] = {
                .vertices{ 
                    // base
                    -1.0f, -1.0f,  1.0f,
                     1.0f, -1.0f,  1.0f,
                    -1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,
                    // front
                    -1.0f, -1.0f,  1.0f,
                     1.0f, -1.0f,  1.0f,
                     0.0f,  1.0f,  0.0f,
                    // back
                    -1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,
                     0.0f,  1.0f,  0.0f,
                    // left
                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                     0.0f,  1.0f,  0.0f,
                    // right
                     1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f,  1.0f,
                     0.0f,  1.0f,  0.0f,
                },
                .colors{
                    // base
                    COLOR_RED, COLOR_RED, COLOR_RED, COLOR_RED,
                    // front
                    COLOR_GREEN, COLOR_GREEN, COLOR_GREEN,
                    // back
                    COLOR_BLUE, COLOR_BLUE, COLOR_BLUE,
                    // left
                    COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW,
                    // right
                    COLOR_CYAN, COLOR_CYAN, COLOR_CYAN,
                },
                .indices{
                    2, 1, 0,
                    2, 3, 1,
                    4, 5, 6,
                    8, 7, 9,
                    10, 11, 12,
                    14, 13, 15,
                },
            };
        }
        else {
            Logger::get_default()->warning(std::format("Mesh named {} could not be loaded.", mesh_name));
            return false;
        }
        return true;
    }
};
