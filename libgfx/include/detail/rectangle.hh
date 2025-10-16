#pragma once

#include <array>
#include <vector>

#include <window.hh>
#include <types.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class RectangleRenderer : public IBatchRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_color_buffer;
    GLuint m_index_buffer;
    GLuint m_transform_buffer;

    std::vector<glm::vec2> m_vertices;
    std::vector<glm::vec4> m_colors;
    std::vector<unsigned int> m_indices;
    std::vector<glm::mat4> m_transforms;

public:
    explicit RectangleRenderer(gfx::Window& window);
    void draw(
        int x,
        int y,
        int width,
        int height,
        const gfx::IRotation& rotation,
        gfx::Color color,
        glm::mat4 view
    );
    void flush() override;

};

} // namespace gfx::detail
