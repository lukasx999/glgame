#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <window.hh>
#include <detail/detail.hh>

namespace gfx {

namespace detail {
class TextRenderer;
} // namespace detail

class Font {
    FT_Face m_face;

    friend detail::TextRenderer;

    Font(FT_Library ft, const char* path) {
        if (FT_New_Face(ft, path, 0, &m_face) != 0) {
            throw std::runtime_error("failed to load font");
        }
    }

public:
    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&) = delete;

    ~Font() {
        FT_Done_Face(m_face);
    }

    [[nodiscard]] int measure_char(char c, int size) const {
        return load_glyph(c, size).advance;
    }

    [[nodiscard]] int measure_text(const char* text, int size) const {
        int result = 0;

        for (const char* c = text; *c; ++c) {
            result += measure_char(*c, size);
        }

        return result;
    }

private:
    [[nodiscard]] detail::Glyph load_glyph(char c, int size) const {

        if (FT_Set_Pixel_Sizes(m_face, 0, size)) {
            throw std::runtime_error("failed to set pixel size");
        }

        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER) != 0) {
            throw std::runtime_error("failed to load char");
        }

        auto glyph = m_face->glyph;
        unsigned int width = glyph->bitmap.width;
        unsigned int height = glyph->bitmap.rows;
        int bearing_x = glyph->bitmap_left;
        int bearing_y = glyph->bitmap_top;
        unsigned int advance = glyph->advance.x;
        unsigned char* buffer = glyph->bitmap.buffer;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        GLuint texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buffer);

        glBindTexture(GL_TEXTURE_2D, 0);

        return {
            texture,
            width,
            height,
            bearing_x,
            bearing_y,
            // advance is pixels * 64
            advance / 64,
            buffer,
        };
    }

};

} // namespace gfx

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
        glm::vec2(1.0, 1.0), // top-right
        glm::vec2(0.0, 1.0), // top-left
        glm::vec2(0.0, 0.0), // bottom-left
        glm::vec2(1.0, 0.0), // bottom-right
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
    TextRenderer(const TextRenderer&) = delete;
    TextRenderer(TextRenderer&&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    TextRenderer& operator=(TextRenderer&&) = delete;

    void draw(int x, int y, int text_size, const char* text, const gfx::Font& font, gfx::Color color);

    [[nodiscard]] gfx::Font load_font(const char* path) const {
        return { m_ft, path };
    }

private:
    void draw_char(int x, int y, const Glyph& glyph, gfx::Color color);

};

} // namespace gfx::detail
