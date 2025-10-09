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
    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_tex_coord_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_tex_coord_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_tex_coords.size() * sizeof(glm::vec2), m_tex_coords.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    m_program = create_shader_program(texture_shader_vertex_src, texture_shader_fragment_src);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_position)));
    glEnableVertexAttribArray(a_pos);

    GLint a_uv = glGetAttribLocation(m_program, "a_uv");
    glVertexAttribPointer(a_uv, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_uv);

    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextureRenderer::draw(
    int x,
    int y,
    int width,
    int height,
    float rotation_deg,
    const gfx::Texture& texture
) const {

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE0);

    GLuint gl_texture;
    glGenTextures(1, &gl_texture);
    glBindTexture(GL_TEXTURE_2D, gl_texture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        texture.get_width(),
        texture.get_height(),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        texture.get_data()
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(x+width/2, y+height/2, 0.0f));
    model = glm::rotate(model, glm::radians(rotation_deg), glm::vec3(0.0f, 0.0f, 1.0f));
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

    glUseProgram(m_program);

    GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
    glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

    glBindVertexArray(m_vertex_array);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

} // namespace detail
