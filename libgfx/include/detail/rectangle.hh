#pragma once

#include <array>
#include <stdexcept>

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <types.hh>

namespace detail {

struct Vertex {
    glm::vec2 m_position;
};

inline constexpr char shader_vertex_src[] = {
    #embed "../../shaders/shader.vert"
};

inline constexpr char shader_fragment_src[] = {
    #embed "../../shaders/shader.frag"
};

class RectangleRenderer {
    GLFWwindow* m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_index_buffer;

    static constexpr std::array m_vertices {
        Vertex({ 1.0, 1.0 }), // top-right
        Vertex({ 0.0, 1.0 }), // top-left
        Vertex({ 0.0, 0.0 }), // bottom-left
        Vertex({ 1.0, 0.0 }), // bottom-right
    };

    static constexpr std::array m_indices {
        0u, 1u, 2u,
        3u, 2u, 0u,
    };

public:
    explicit RectangleRenderer(GLFWwindow* window);

    void draw_rectangle(int x, int y, int width, int height, gfx::Color color) const {

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(x, y, 0.0f));
        model = glm::scale(model, glm::vec3(width, height, 0.0f));

        // TODO: camera
        // glm::vec3 camera_position(0.0f, 0.0f, 0.0f);
        // glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
        // glm::vec3 up(0.0f, 1.0f, 0.0f);
        // glm::mat4 view = glm::lookAt(camera_position, camera_position+camera_direction, up);

        glm::mat4 view(1.0f);

        // TODO:
        int fb_width, fb_height;
        glfwGetFramebufferSize(m_window, &fb_width, &fb_height);

        glm::mat4 projection = glm::ortho(
            0.0f,
            static_cast<float>(fb_width),
            static_cast<float>(fb_height),
            0.0f
        );

        glm::mat4 mvp = projection * view * model;

        glUseProgram(m_program);

        GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
        glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

        GLint u_color = glGetUniformLocation(m_program, "u_color");
        auto normalized = color.normalized();
        glUniform4f(u_color, normalized.r, normalized.g, normalized.b, normalized.a);

        glBindVertexArray(m_vertex_array);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }

private:
    [[nodiscard]] static GLuint create_shader_program() {

        GLuint vertex_shader = load_shader(GL_VERTEX_SHADER);
        GLuint fragment_shader = load_shader(GL_FRAGMENT_SHADER);
        GLuint program = glCreateProgram();

        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);

        glLinkProgram(program);
        glUseProgram(program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }

    [[nodiscard]] static GLuint load_shader(GLenum type) {
        GLuint vertex_shader = glCreateShader(type);

        const char* src = [&]() {
            switch (type) {
                case GL_FRAGMENT_SHADER:
                    return shader_fragment_src;
                    break;

                case GL_VERTEX_SHADER:
                    return shader_vertex_src;
                    break;
            }

            throw std::runtime_error("unknown opengl shader type");
        }();

        glShaderSource(vertex_shader, 1, &src, nullptr);
        glCompileShader(vertex_shader);

        return vertex_shader;
    }

};

} // namespace detail
