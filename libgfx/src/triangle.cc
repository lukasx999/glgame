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

    m_program = detail::create_shader_program(shaders::vertex::rectangle, shaders::fragment::rectangle);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);


    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_pos);


    glGenBuffers(1, &m_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);

    GLint a_color = glGetAttribLocation(m_program, "a_color");
    glVertexAttribPointer(a_color, 4, GL_FLOAT, false, sizeof(glm::vec4), nullptr);
    glEnableVertexAttribArray(a_color);


    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleRenderer::draw(int x0, int y0, int x1, int y1, int x2, int y2, gfx::Color color) {
    m_vertices.push_back({ x_to_ndc(m_window, x0), y_to_ndc(m_window, y0) });
    m_vertices.push_back({ x_to_ndc(m_window, x1), y_to_ndc(m_window, y1) });
    m_vertices.push_back({ x_to_ndc(m_window, x2), y_to_ndc(m_window, y2) });

    auto c = color.normalized();
    glm::vec4 cv(c.r, c.g, c.b, c.a);
    m_colors.push_back(cv);
    m_colors.push_back(cv);
    m_colors.push_back(cv);
}

void TriangleRenderer::flush() {

    glBindVertexArray(m_vertex_array);
    glUseProgram(m_program);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() *  sizeof(glm::vec2), m_vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_colors.size() *  sizeof(glm::vec4), m_colors.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

    m_colors.clear();
    m_vertices.clear();
}

} // namespace gfx::detail
