#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/line.hh>
#include "shaders.hh"

namespace detail {

LineRenderer::LineRenderer(gfx::Window& window)
: m_window(window)
{

    m_program = create_shader_program(shaders::vertex::default_, shaders::fragment::default_);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_position)));
    glEnableVertexAttribArray(a_pos);

    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineRenderer::draw(int x0, int y0, int x1, int y1, gfx::Color color) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    std::array vertices {
        Vertex({ x0, y0 }),
        Vertex({ x1, y1 }),
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;
    GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
    glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

    GLint u_color = glGetUniformLocation(m_program, "u_color");
    auto c = color.normalized();
    glUniform4f(u_color, c.r, c.g, c.b, c.a);

    glDrawArrays(GL_LINES, 0, vertices.size());

}

} // namespace detail
