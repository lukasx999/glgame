#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/circle.hh>
#include "shaders.hh"

namespace gfx::detail {

CircleRenderer::CircleRenderer(gfx::Window& window)
: m_window(window)
{
    m_program = create_shader_program(shaders::vertex::default_, shaders::fragment::circle);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);


    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec2), m_vertices.data(), GL_STATIC_DRAW);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_pos);


    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);


    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CircleRenderer::draw(int x, int y, int radius, gfx::Color color) {

    glBindVertexArray(m_vertex_array);
    glUseProgram(m_program);

    glm::mat4 model(1.0f);
    // square is placed at the center of the circle
    model = glm::translate(model, glm::vec3(x-radius, y-radius, 0.0f));
    model = glm::scale(model, glm::vec3(radius*2, radius*2, 0.0f));

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * model;

    GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
    glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

    GLint u_window_height = glGetUniformLocation(m_program, "u_window_height");
    glUniform1i(u_window_height, m_window.get_height());

    GLint u_radius = glGetUniformLocation(m_program, "u_radius");
    glUniform1i(u_radius, radius);

    GLint u_center = glGetUniformLocation(m_program, "u_center");
    glUniform2f(u_center, x, y);

    GLint u_color = glGetUniformLocation(m_program, "u_color");
    auto normalized = color.normalized();
    glUniform4f(u_color, normalized.r, normalized.g, normalized.b, normalized.a);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

} // namespace gfx::detail
