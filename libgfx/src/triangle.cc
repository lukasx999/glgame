#include <array>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/triangle.hh>
#include "shaders.hh"

namespace gfx::detail {

TriangleRenderer::TriangleRenderer(gfx::Window& window)
: m_window(window)
{

    m_program = detail::create_shader_program(shaders::vertex::default_, shaders::fragment::default_);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);


    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_position)));
    glEnableVertexAttribArray(a_pos);


    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleRenderer::draw(int x0, int y0, int x1, int y1, int x2, int y2, gfx::Color color) {

    glBindVertexArray(m_vertex_array);
    glUseProgram(m_program);

    std::array vertices {
        Vertex({ x0, y0 }),
        Vertex({ x1, y1 }),
        Vertex({ x2, y2 }),
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() *  sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
    glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(projection));

    GLint u_color = glGetUniformLocation(m_program, "u_color");
    auto normalized = color.normalized();
    glUniform4f(u_color, normalized.r, normalized.g, normalized.b, normalized.a);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

} // namespace gfx::detail
