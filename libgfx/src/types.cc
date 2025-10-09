#include <types.hh>

#include <stb_image.h>

namespace gfx {

Texture::Texture(std::filesystem::path path) {
    m_data = stbi_load(path.c_str(), &m_width, &m_height, nullptr, 0);
}

Texture::~Texture() {
    stbi_image_free(m_data);
}

} // namespace gfx
