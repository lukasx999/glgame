#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stb_image.h>
#include <GLFW/glfw3.h>

#include <detail/texture.hh>
#include "shaders.hh"

namespace gfx {

Texture::Texture(const char* path) {
    m_data = stbi_load(path, &m_width, &m_height, &m_channels, 0);
    if (m_data == nullptr) {
        // TODO: custom exception type
        throw std::runtime_error("failed to load texture");
    }

    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint format = get_opengl_texture_format();
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    stbi_image_free(m_data);
    glDeleteTextures(1, &m_texture);
}

namespace detail {

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

void TextureRenderer::draw(
    int x,
    int y,
    int width,
    int height,
    const gfx::IRotation& rotation,
    const gfx::Texture& texture,
    glm::mat4 view
) {

    // the translations are needed to subtract the world space coordinates (x,y)
    // since we want the rectangle to rotate around its top left corner, and
    // to set the center of rotation to the middle of the rectangle
    glm::mat4 model(1.0);
    model = glm::translate(model, glm::vec3(x+width/2.0, y+height/2.0, 0.0));
    model = glm::rotate(model, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-x-width/2.0, -y-height/2.0, 0.0));

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;

    if (m_render_group.contains(texture.m_data)) {
        RenderGroup& g = m_render_group.at(texture.m_data);

        int vertex_count = g.vertices.size();
        g.indices.push_back(0u + vertex_count); // top-left
        g.indices.push_back(1u + vertex_count); // top-right
        g.indices.push_back(2u + vertex_count); // bottom-left
        g.indices.push_back(3u + vertex_count); // bottom-right
        g.indices.push_back(2u + vertex_count); // bottom-left
        g.indices.push_back(1u + vertex_count); // top-right

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
        m_render_group.emplace(texture.m_data, group);
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

    m_render_group.clear();
}

} // namespace detail

} // namespace gfx
