#pragma once

#include <print>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace detail {

struct Vertex {
    glm::vec2 m_position;
};

[[nodiscard]] inline GLuint create_shader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512] = {0};
        glGetShaderInfoLog(shader, sizeof info_log, nullptr, info_log);
        std::println(stderr, "shader compilation failed: {}", info_log);
        exit(EXIT_FAILURE);
    }

    return shader;
}

[[nodiscard]] inline GLuint create_shader_program(const char* vertex_src, const char* fragment_src) {

    GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_src);
    GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_src);
    GLuint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    glUseProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512] = {0};
        glGetProgramInfoLog(program, sizeof info_log, nullptr, info_log);
        std::println(stderr, "shader program linkage failed: {}", info_log);
        exit(EXIT_FAILURE);
    }

    return program;
}

} // namespace detail
