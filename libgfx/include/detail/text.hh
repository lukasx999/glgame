#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <window.hh>
#include <font.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class TextRenderer {
    gfx::Window& m_window;

    FT_Library m_ft;

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
        glm::vec2(1.0, 1.0), // top-right
        glm::vec2(0.0, 1.0), // top-left
        glm::vec2(0.0, 0.0), // bottom-left
        glm::vec2(1.0, 0.0), // bottom-right
    };

    static constexpr std::array m_indices {
        0u, 1u, 2u,
        3u, 2u, 0u,
    };

public:
    TextRenderer(gfx::Window& window);
    ~TextRenderer();
    void draw(int x, int y, int text_size, const char* text, const gfx::Font& font);

    [[nodiscard]] gfx::Font load_font(const char* path) const {
        return { m_ft, path };
    }

private:
    void draw_char(int x, int y, const Glyph& glyph);

};

} // namespace gfx::detail
