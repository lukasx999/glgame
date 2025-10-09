#include <types.hh>

#include <stb_image.h>

namespace gfx {

Texture::Texture(std::filesystem::path path) {
    // opengl flips the y-axis of textures, so just flip them when loading
    stbi_set_flip_vertically_on_load(true);
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    if (m_data == nullptr) {
        // TODO: custom exception type
        throw std::runtime_error("failed to load texture");
    }
}

Texture::~Texture() {
    stbi_image_free(m_data);
}

} // namespace gfx
