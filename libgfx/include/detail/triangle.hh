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
        glm::vec2 m_position;
        int m_vertex_no;
    };

    static constexpr std::array m_vertices {
        TriangleVertex({ 0.0, 0.0 }, 0),
        TriangleVertex({ 1.0, 0.0 }, 1),
        TriangleVertex({ 0.0, 1.0 }, 2),
    };

public:
    explicit TriangleRenderer(GLFWwindow* window);

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

        glm::mat4 model0(1.0f);
        model0 = glm::translate(model0, glm::vec3(x0, y0, 0.0f));
        GLint u_model0 = glGetUniformLocation(m_program, "u_model0");
        glUniformMatrix4fv(u_model0, 1, false, glm::value_ptr(model0));

        glm::mat4 model1(1.0f);
        model1 = glm::translate(model1, glm::vec3(x1, y1, 0.0f));
        GLint u_model1 = glGetUniformLocation(m_program, "u_model1");
        glUniformMatrix4fv(u_model1, 1, false, glm::value_ptr(model1));

        glm::mat4 model2(1.0f);
        model2 = glm::translate(model2, glm::vec3(x2, y2, 0.0f));
        GLint u_model2 = glGetUniformLocation(m_program, "u_model2");
        glUniformMatrix4fv(u_model2, 1, false, glm::value_ptr(model2));


        // GLint u_position0 = glGetUniformLocation(m_program, "u_position0");
        // glUniform2f(u_position0, x0, y0);
        //
        // GLint u_position0 = glGetUniformLocation(m_program, "u_position0");
        // glUniform2f(u_position0, x0, y0);
        //
        // GLint u_position1 = glGetUniformLocation(m_program, "u_position1");
        // glUniform2f(u_position1, x1, y1);
        //
        // GLint u_position2 = glGetUniformLocation(m_program, "u_position2");
        // glUniform2f(u_position2, x2, y2);

        GLint u_color = glGetUniformLocation(m_program, "u_color");
        auto normalized = color.normalized();
        glUniform4f(u_color, normalized.r, normalized.g, normalized.b, normalized.a);

        glBindVertexArray(m_vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    }

};

} // namespace detail
