#pragma once

#include <glad/gl.h>
#include <stdexcept>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace gfx {

namespace detail {
class TextRenderer;
} // namespace detail

// TODO: should probably be private
struct Glyph {
    GLuint texture;
    unsigned int width;
    unsigned int height;
    int bearing_x;
    int bearing_y;
    unsigned int advance;
    unsigned char* buffer;
};

// TODO: text measurement
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

    [[nodiscard]] Glyph load_glyph(char c, int size) const {

        if (FT_Set_Pixel_Sizes(m_face, 0, size)) {
            throw std::runtime_error("failed to set pixel size");
        }

        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER) != 0) {
            throw std::runtime_error("failed to load char");
        }

        unsigned int width = m_face->glyph->bitmap.width;
        unsigned int height = m_face->glyph->bitmap.rows;
        int bearing_x = m_face->glyph->bitmap_left;
        int bearing_y = m_face->glyph->bitmap_top;
        unsigned int advance = m_face->glyph->advance.x;
        unsigned char* buffer = m_face->glyph->bitmap.buffer;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        GLuint texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);

        glBindTexture(GL_TEXTURE_2D, 0);

        return Glyph {
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
