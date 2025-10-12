#pragma once

#include <stdexcept>

#include <glad/gl.h>

namespace gfx {

namespace detail {
class TextureRenderer;
} // namespace detail

class Texture {
    GLuint m_texture;
    int m_width;
    int m_height;
    int m_channels;
    unsigned char* m_data;

    friend detail::TextureRenderer;

public:
    Texture(const char* path);
    ~Texture();

    [[nodiscard]] int get_width() const {
        return m_width;
    }

    [[nodiscard]] int get_height() const {
        return m_height;
    }

    [[nodiscard]] int get_channels() const {
        return m_channels;
    }

    // TODO: copy/move ctor
    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

private:
    [[nodiscard]] constexpr GLint get_opengl_texture_format() const {
        switch (m_channels) {
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
        }
        throw std::runtime_error("invalid channel count");
    }

};

} // namespace gfx
