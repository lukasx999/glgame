#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <window.hh>
#include <detail/detail.hh>

namespace detail {

class TextRenderer {
    gfx::Window& m_window;

    FT_Library m_ft;
    FT_Face m_face;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_uv_buffer;
    GLuint m_index_buffer;

    static constexpr std::array m_vertices {
        Vertex({ 1.0, 1.0 }), // top-right
        Vertex({ 0.0, 1.0 }), // top-left
        Vertex({ 0.0, 0.0 }), // bottom-left
        Vertex({ 1.0, 0.0 }), // bottom-right
    };

    static constexpr std::array m_uvs {
        glm::vec2(1.0, 0.0),
        glm::vec2(0.0, 0.0),
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 1.0),
    };

    static constexpr std::array m_indices {
        0u, 1u, 2u,
        3u, 2u, 0u,
    };

public:
    TextRenderer(gfx::Window& window);
    void draw(int x, int y, const char* text);

};

} // namespace detail
