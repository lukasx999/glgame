#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/texture.hh>
#include "shaders.hh"

namespace gfx::detail {

TextureRenderer::TextureRenderer(gfx::Window& window)
: m_window(window)
{
    m_program = create_shader_program(shaders::vertex::texture, shaders::fragment::texture);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_index_buffer);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_pos);

    glGenBuffers(1, &m_uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);
    GLint a_uv = glGetAttribLocation(m_program, "a_uv");
    glVertexAttribPointer(a_uv, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_uv);

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

void TextureRenderer::draw(int x, int y, int width, int height, const gfx::IRotation& rotation, const gfx::Texture& texture, glm::mat4 view) {

    glm::mat4 model(1.0);

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;


    if (m_group_data.contains(texture.m_data)) {
        RenderGroup& g = m_group_data.at(texture.m_data);

        g.indices.push_back(0u + g.vertices.size()); // top-left
        g.indices.push_back(1u + g.vertices.size()); // top-right
        g.indices.push_back(2u + g.vertices.size()); // bottom-left
        g.indices.push_back(3u + g.vertices.size()); // bottom-right
        g.indices.push_back(2u + g.vertices.size()); // bottom-left
        g.indices.push_back(1u + g.vertices.size()); // top-right

        g.vertices.push_back({ x,       y        }); // top-left
        g.vertices.push_back({ x+width, y        }); // top-right
        g.vertices.push_back({ x,       y+height }); // bottom-left
        g.vertices.push_back({ x+width, y+height }); // bottom-right

        g.uvs.push_back({ 0.0, 0.0 }); // top-left
        g.uvs.push_back({ 1.0, 0.0 }); // top-right
        g.uvs.push_back({ 0.0, 1.0 }); // bottom-left
        g.uvs.push_back({ 1.0, 1.0 }); // bottom-right

        g.transforms.push_back(mvp);
        g.transforms.push_back(mvp);
        g.transforms.push_back(mvp);
        g.transforms.push_back(mvp);

    } else {
        RenderGroup group {
            texture,
            {
                { x,       y        }, // top-left
                { x+width, y        }, // top-right
                { x,       y+height }, // bottom-left
                { x+width, y+height }, // bottom-right
            },
            {
                0u, // top-left
                1u, // top-right
                2u, // bottom-left
                3u, // bottom-right
                2u, // bottom-left
                1u, // top-right
            },
            {
                { 0.0, 0.0 }, // top-left
                { 1.0, 0.0 }, // top-right
                { 0.0, 1.0 }, // bottom-left
                { 1.0, 1.0 }, // bottom-right
            },
            {
                mvp,
                mvp,
                mvp,
                mvp,
            }
        };
        m_group_data.emplace(texture.m_data, group);
    }

}

void TextureRenderer::flush() {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    for (auto& [key, value] : m_group_data) {
        auto& [texture, vertices, indices, uvs, transforms] = value;

        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_transform_buffer);
        glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_DYNAMIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, texture.m_texture);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }

    m_group_data.clear();
}

} // namespace gfx::detail
