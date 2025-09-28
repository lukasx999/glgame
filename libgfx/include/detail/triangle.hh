#pragma once

#include <print>

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <types.hh>
#include <detail/detail.hh>

namespace detail {

class TriangleRenderer {
    GLFWwindow* m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;

    struct TriangleVertex {
        int m_vertex_no;
    };

    static constexpr std::array m_vertices {
        TriangleVertex(0),
        TriangleVertex(1),
        TriangleVertex(2),
    };

public:
    TriangleRenderer(GLFWwindow* window);

    void draw(int x0, int y0, int x1, int y1, int x2, int y2, gfx::Color color) {

        // TODO:
        int fb_width, fb_height;
        glfwGetFramebufferSize(m_window, &fb_width, &fb_height);

        glm::mat4 projection = glm::ortho(
            0.0f,
            static_cast<float>(fb_width),
            static_cast<float>(fb_height),
            0.0f
        );

        glUseProgram(m_program);

        GLint u_projection = glGetUniformLocation(m_program, "u_projection");
        glUniformMatrix4fv(u_projection, 1, false, glm::value_ptr(projection));

        GLint u_position0 = glGetUniformLocation(m_program, "u_position0");
        glUniform2f(u_position0, x0, y0);

        GLint u_position1 = glGetUniformLocation(m_program, "u_position1");
        glUniform2f(u_position1, x1, y1);

        GLint u_position2 = glGetUniformLocation(m_program, "u_position2");
        glUniform2f(u_position2, x2, y2);

        GLint u_color = glGetUniformLocation(m_program, "u_color");
        auto normalized = color.normalized();
        glUniform4f(u_color, normalized.r, normalized.g, normalized.b, normalized.a);

        glBindVertexArray(m_vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    }

};

} // namespace detail
