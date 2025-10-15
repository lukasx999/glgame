#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/rectangle.hh>
#include "shaders.hh"

namespace gfx::detail {

RectangleRenderer::RectangleRenderer(gfx::Window& window)
: m_window(window)
{

    m_program = create_shader_program(shaders::vertex::batched, shaders::fragment::batched);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_index_buffer);

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


    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// TODO: what about rotation/camera? (we probably still need a matrix)
void RectangleRenderer::draw(int x, int y, int width, int height, const gfx::IRotation& rotation, gfx::Color color) {

    m_indices.push_back(0u + m_vertices.size()); // top-left
    m_indices.push_back(1u + m_vertices.size()); // top-right
    m_indices.push_back(2u + m_vertices.size()); // bottom-left
    m_indices.push_back(3u + m_vertices.size()); // bottom-right
    m_indices.push_back(2u + m_vertices.size()); // bottom-left
    m_indices.push_back(1u + m_vertices.size()); // top-right

    m_vertices.push_back({ x_to_ndc(m_window, x),       y_to_ndc(m_window, y)        }); // top-left
    m_vertices.push_back({ x_to_ndc(m_window, x+width), y_to_ndc(m_window, y)        }); // top-right
    m_vertices.push_back({ x_to_ndc(m_window, x),       y_to_ndc(m_window, y+height) }); // bottom-left
    m_vertices.push_back({ x_to_ndc(m_window, x+width), y_to_ndc(m_window, y+height) }); // bottom-right

    auto c = color.normalized();
    glm::vec4 cv(c.r, c.g, c.b, c.a);

    m_colors.push_back(cv);
    m_colors.push_back(cv);
    m_colors.push_back(cv);
    m_colors.push_back(cv);

}

void RectangleRenderer::flush() {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec2), m_vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec4), m_colors.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

    m_vertices.clear();
    m_indices.clear();
    m_colors.clear();
}

} // namespace gfx::detail
