#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/rectangle.hh>
#include "shaders.hh"

namespace detail {

RectangleRenderer::RectangleRenderer(gfx::Window& window)
: m_window(window)
{

    m_program = create_shader_program(rectangle_shader_vertex_src, rectangle_shader_fragment_src);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_position)));
    glEnableVertexAttribArray(a_pos);

    glGenBuffers(1, &m_instanced_array);
    glBindBuffer(GL_ARRAY_BUFFER, m_instanced_array);

    GLint a_mvp = glGetAttribLocation(m_program, "a_mvp");
    for (int i=0; i < 4; ++i) {
        glEnableVertexAttribArray(a_mvp+i);
        glVertexAttribPointer(
            a_mvp+i,
            4,
            GL_FLOAT,
            false,
            sizeof(InstanceData),
            reinterpret_cast<void*>(offsetof(InstanceData, mvp) + sizeof(float)*4*i)
        );
        glVertexAttribDivisor(a_mvp+i, 1);
    }

    GLint a_color = glGetAttribLocation(m_program, "a_color");
    glEnableVertexAttribArray(a_color);
    glVertexAttribPointer(
        a_color,
        4,
        GL_FLOAT,
        false,
        sizeof(InstanceData),
        reinterpret_cast<void*>(offsetof(InstanceData, color))
    );
    glVertexAttribDivisor(a_color, 1);

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

void RectangleRenderer::draw(int x, int y, int width, int height, float rotation_deg, gfx::Color color) {

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(x+width/2, y+height/2, 0.0f));
    model = glm::rotate(model, glm::radians(rotation_deg), glm::vec3(0.0f, 0.0f, 1.0f));
    // subtract half of width & height, needed for centered rotation
    model = glm::translate(model, glm::vec3(-width/2, -height/2, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 0.0f));

    // TODO: camera
    // glm::vec3 camera_position(0.0f, 0.0f, 0.0f);
    // glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
    // glm::vec3 up(0.0f, 1.0f, 0.0f);
    // glm::mat4 view = glm::lookAt(camera_position, camera_position+camera_direction, up);

    glm::mat4 view(1.0f);

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;

    auto c = color.normalized();
    InstanceData data(mvp, glm::vec4(c.r, c.g, c.b, c.a));
    m_instance_data.push_back(std::move(data));
}

void RectangleRenderer::flush() {

    glBindBuffer(GL_ARRAY_BUFFER, m_instanced_array);
    glBufferData(GL_ARRAY_BUFFER, m_instance_data.size() * sizeof(InstanceData), m_instance_data.data(), GL_STATIC_DRAW);

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    glDrawElementsInstanced(
        GL_TRIANGLES,
        m_indices.size(),
        GL_UNSIGNED_INT,
        nullptr,
        m_instance_data.size()
    );
    m_instance_data.clear();
}

} // namespace detail
