#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/texture.hh>
#include "shaders.hh"

namespace detail {

TextureRenderer::TextureRenderer(gfx::Window& window)
: m_window(window)
{
    m_program = create_shader_program(shaders::vertex::texture, shaders::fragment::texture);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);


    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_position)));
    glEnableVertexAttribArray(a_pos);


    glGenBuffers(1, &m_tex_coord_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_tex_coord_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_tex_coords.size() * sizeof(glm::vec2), m_tex_coords.data(), GL_STATIC_DRAW);

    GLint a_uv = glGetAttribLocation(m_program, "a_uv");
    glVertexAttribPointer(a_uv, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_uv);

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);


    glGenBuffers(1, &m_transform_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_transform_buffer);

    GLint a_mvp = glGetAttribLocation(m_program, "a_mvp");
    for (int i=0; i < 4; ++i) {
        glEnableVertexAttribArray(a_mvp+i);
        glVertexAttribPointer(
            a_mvp+i,
            4,
            GL_FLOAT,
            false,
            sizeof(glm::mat4),
            reinterpret_cast<void*>(sizeof(float)*4*i)
        );
        glVertexAttribDivisor(a_mvp+i, 1);
    }

    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextureRenderer::draw(int x, int y, int width, int height, const gfx::IRotation& rotation, const gfx::Texture& texture) {

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(x+width/2, y+height/2, 0.0f));
    model = glm::rotate(model, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    // subtract half of width & height, needed for centered rotation
    model = glm::translate(model, glm::vec3(-width/2, -height/2, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 0.0f));

    glm::mat4 view(1.0f);

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;

    if (m_group_data.contains(texture.get_data())) {
        RenderGroup& group = m_group_data.at(texture.get_data());
        group.transforms.push_back(std::move(mvp));

    } else {
        m_group_data.emplace(texture.get_data(), RenderGroup(texture, {mvp}));
    }

}

void TextureRenderer::flush() {

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    GLuint gl_texture;
    glGenTextures(1, &gl_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gl_texture);

    for (auto& [key, value] : m_group_data) {
        auto& [texture, transforms] = value;

        glBindBuffer(GL_ARRAY_BUFFER, m_transform_buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            transforms.size() * sizeof(glm::mat4),
            transforms.data(),
            GL_STATIC_DRAW
        );

        GLint format = get_opengl_texture_format(texture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            texture.get_width(),
            texture.get_height(),
            0,
            format,
            GL_UNSIGNED_BYTE,
            texture.get_data()
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        glDrawElementsInstanced(
            GL_TRIANGLES,
            m_indices.size(),
            GL_UNSIGNED_INT,
            nullptr,
            transforms.size()
        );
    }

    m_group_data.clear();
}

} // namespace detail
