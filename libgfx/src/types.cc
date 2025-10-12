#include <texture.hh>

#include <stb_image.h>

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

} // namespace gfx
