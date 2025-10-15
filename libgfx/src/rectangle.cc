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

    glGenBuffers(1, &m_transform_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_transform_buffer);
    GLint a_mvp = glGetAttribLocation(m_program, "a_mvp");
    for (int i = 0; i < 4; ++i) {
        glVertexAttribPointer(a_mvp+i, 4, GL_FLOAT, false, sizeof(glm::vec4)*4, reinterpret_cast<void*>(i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(a_mvp+i);
    }


    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RectangleRenderer::draw(int x, int y, int width, int height, const gfx::IRotation& rotation, gfx::Color color, glm::mat4 view) {

    int vertex_count =  m_vertices.size();
    m_indices.push_back(0u + vertex_count); // top-left
    m_indices.push_back(1u + vertex_count); // top-right
    m_indices.push_back(2u + vertex_count); // bottom-left
    m_indices.push_back(3u + vertex_count); // bottom-right
    m_indices.push_back(2u + vertex_count); // bottom-left
    m_indices.push_back(1u + vertex_count); // top-right

    m_vertices.push_back({ x,       y        }); // top-left
    m_vertices.push_back({ x+width, y        }); // top-right
    m_vertices.push_back({ x,       y+height }); // bottom-left
    m_vertices.push_back({ x+width, y+height }); // bottom-right

    // TODO: doesnt work yet, as the rotation transform matrix is also applied to
    // x+width, leading to weird rotations
    // the old instance rendering implementation was using x+1.0
    // either: use instance rendering like before, or use different transforms
    // for different vertices of the rectangle
    glm::mat4 model(1.0);
    // subtract half of width & height, needed for centered rotation
    // model = glm::translate(model, glm::vec3(-width/2.0f, -height/2.0f, 0.0f));
    // model = glm::rotate(model, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    // model = glm::translate(model, glm::vec3(width/2.0f, height/2.0f, 0.0f));

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;

    m_transforms.push_back(mvp);
    m_transforms.push_back(mvp);
    m_transforms.push_back(mvp);
    m_transforms.push_back(mvp);

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

    glBindBuffer(GL_ARRAY_BUFFER, m_transform_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_transforms.size() * sizeof(glm::mat4), m_transforms.data(), GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

    m_vertices.clear();
    m_indices.clear();
    m_colors.clear();
    m_transforms.clear();
}

} // namespace gfx::detail
