
#pragma once

#include <array>
#include <functional>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <types.hh>
#include <detail/detail.hh>

namespace detail {

class CircleRenderer {
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
    explicit CircleRenderer(GLFWwindow* window);

    // TODO: is this useful?
    // template <typename... Args>
    // static void set_uniform(GLuint program, const char* name, std::function<void(Args...)> fn, Args&& ...args) {
    //     GLint location = glGetUniformLocation(program, name);
    //     fn(location, std::forward<Args>(args)...);
    // }

    void draw(int x, int y, int radius, gfx::Color color) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(x-radius, y-radius, 0.0f));
        model = glm::scale(model, glm::vec3(radius*2, radius*2, 0.0f));

        // TODO:
        int fb_width, fb_height;
        glfwGetFramebufferSize(m_window, &fb_width, &fb_height);

        glm::mat4 projection = glm::ortho(
            0.0f,
            static_cast<float>(fb_width),
            static_cast<float>(fb_height),
            0.0f
        );

        glm::mat4 mvp = projection * model;

        glUseProgram(m_program);

        GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
        glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

        GLint u_window_height = glGetUniformLocation(m_program, "u_window_height");
        glUniform1i(u_window_height, fb_height);

        GLint u_radius = glGetUniformLocation(m_program, "u_radius");
        glUniform1i(u_radius, radius);

        GLint u_center = glGetUniformLocation(m_program, "u_center");
        glUniform2f(u_center, x, y);

        GLint u_color = glGetUniformLocation(m_program, "u_color");
        auto normalized = color.normalized();
        glUniform4f(u_color, normalized.r, normalized.g, normalized.b, normalized.a);

        glBindVertexArray(m_vertex_array);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }

};

} // namespace detail
