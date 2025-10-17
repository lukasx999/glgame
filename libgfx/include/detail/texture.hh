#pragma once

#include <unordered_map>
#include <vector>

#include <window.hh>
#include <detail/detail.hh>

namespace gfx {

namespace detail {
class TextureRenderer;
} // namespace detail

class Texture {
    friend detail::TextureRenderer;

    GLuint m_texture;
    int m_width;
    int m_height;
    int m_channels;
    unsigned char* m_data;

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

namespace gfx::detail {

class TextureRenderer : public IDeferredRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_index_buffer;
    GLuint m_uv_buffer;
    GLuint m_transform_buffer;

    struct RenderGroup {
        const gfx::Texture& texture;
        std::vector<glm::vec2> vertices;
        std::vector<unsigned int> indices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::mat4> transforms;
    };

    // the texture sampler is a uniform, so we create groups of transforms per texture
    // and batch-render each group, resulting in one draw call per texture
    //
    // for identifying each group, we just hash the raw pointer to the heap-allocated
    // memory of the texture
    // this means that theoretically 2 identical textures could end up being in a
    // different group, if the user has 2 seperate `gfx::Texture` objects
    // however, this should never be a problem in practice
    std::unordered_map<unsigned char*, RenderGroup> m_render_group;

public:
    explicit TextureRenderer(gfx::Window& window);

    void draw(
        float x,
        float y,
        float width,
        float height,
        const gfx::IRotation& rotation,
        const gfx::Texture& texture,
        glm::mat4 view
    );
    void flush() override;

};

} // namespace gfx::detail
