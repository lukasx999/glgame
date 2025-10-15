#pragma once

#include <array>
#include <unordered_map>

#include <window.hh>
#include <texture.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class TextureRenderer : public IBatchRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_index_buffer;
    GLuint m_uv_buffer;

    struct RenderGroup {
        const gfx::Texture& texture;
        std::vector<glm::vec2> vertices;
        std::vector<unsigned int> indices;
        std::vector<glm::vec2> uvs;
    };

    // the texture sampler is a uniform, so we create groups of transforms per texture
    // and instance-render each group, resulting in one draw call per texture
    //
    // for identifying each group, we just hash the raw pointer to the heap-allocated
    // memory of the texture
    // this means that theoretically 2 identical textures could end up being in a
    // different group, if the user has 2 seperate `gfx::Texture` objects
    // however, this should never be a problem in practice
    std::unordered_map<unsigned char*, RenderGroup> m_group_data;

public:
    explicit TextureRenderer(gfx::Window& window);

    void draw(int x, int y, int width, int height, const gfx::IRotation& rotation, const gfx::Texture& texture);
    void flush() override;

};

} // namespace gfx::detail
