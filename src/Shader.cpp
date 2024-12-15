#include "Shader.hpp"

#include "Logger.hpp"

auto shader::create_shader(std::string_view shader_string, GLenum shader_type) -> std::optional<GLuint>
{
    std::string_view shader_type_string;
    switch (shader_type) {
        case GL_VERTEX_SHADER: {
            shader_type_string = "vertex";
            break;
        }
        case GL_GEOMETRY_SHADER: {
            shader_type_string = "geometry";
            break;
        }
        case GL_FRAGMENT_SHADER: {
            shader_type_string = "fragment";
            break;
        }
        default: {
            shader_type_string = "unknown type";
            break;
        }
    }
    const Logger* logger = Logger::get_default();
    logger->debug(std::format("Compiling {} shader.", shader_type_string));
    GLuint shader = glCreateShader(shader_type);

    const char* shader_source = shader_string.data();
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    GLint compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    GLint info_log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
    std::basic_string<GLchar> info_log = std::basic_string(info_log_length + 1, '\0');
    glGetShaderInfoLog(shader, info_log_length, NULL, info_log.data());

    if (compile_status == GL_FALSE) {
        logger->error(std::format("Compile failure in {} shader: {}", shader_type_string, info_log));
    }
    if (info_log_length > 0) {
        logger->debug(info_log);
    }

    return shader;
}

auto shader::create_shader_program(const std::vector<GLuint>& shader_list) -> std::optional<GLuint>
{
    const Logger* logger = Logger::get_default();
    logger->debug("Compiling shader program.");

    GLuint shader_program = glCreateProgram();
    if (shader_program == 0) {
        logger->error("Error creating shader program.");
        return std::nullopt;
    }

    std::for_each(shader_list.begin(), shader_list.end(),
        [shader_program](GLuint shader) { glAttachShader(shader_program, shader); });
    glLinkProgram(shader_program);

    GLint program_status;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &program_status);
    GLint info_log_length;
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_log_length);
    std::basic_string<GLchar> info_log = std::basic_string(info_log_length + 1, '\0');
    glGetProgramInfoLog(shader_program, info_log_length, NULL, info_log.data());

    if (program_status == GL_FALSE) {
        logger->error(std::format("Shader linker error: {}", info_log));
        return std::nullopt;
    }
    if (info_log_length > 0) {
        logger->debug(info_log);
    }

    std::for_each(shader_list.begin(), shader_list.end(),
        [shader_program](GLuint shader) { glDetachShader(shader_program, shader); });

    return shader_program;
}
