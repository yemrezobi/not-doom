#pragma once

#include <filesystem>
#include <optional>

#include <gl/glew.h>

namespace shader
{
    auto create_shader(std::string_view shader_string, GLenum shader_type) -> std::optional<GLuint>;
    auto create_shader_program(const std::vector<GLuint>& shader_list) -> std::optional<GLuint>;
};
