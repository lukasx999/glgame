#pragma once

#include <array>
#include <unordered_map>

#include <window.hh>
#include <types.hh>
#include <detail/detail.hh>

namespace detail {

class TextureRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_transform_buffer;
    GLuint m_index_buffer;
    GLuint m_tex_coord_buffer;

    static constexpr std::array m_vertices {
        Vertex({ 1.0, 1.0 }), // top-right
        Vertex({ 0.0, 1.0 }), // top-left
        Vertex({ 0.0, 0.0 }), // bottom-left
        Vertex({ 1.0, 0.0 }), // bottom-right
    };

    static constexpr std::array m_tex_coords {
        glm::vec2(1.0, 0.0),
        glm::vec2(0.0, 0.0),
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 1.0),
    };

    static constexpr std::array m_indices {
        0u, 1u, 2u,
        3u, 2u, 0u,
    };

    struct RenderGroup {
        const gfx::Texture& texture;
        std::vector<glm::mat4> transforms;
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

    void draw(int x, int y, int width, int height, float rotation_deg, const gfx::Texture& texture);
    void flush();

};

} // namespace detail
