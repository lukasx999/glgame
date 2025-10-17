#pragma once

#include <array>

#include <types.hh>
#include <window.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class CircleRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_index_buffer;

    static constexpr std::array m_vertices {
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
    explicit CircleRenderer(gfx::Window& window);

    void draw(float x, float y, float radius, gfx::Color color);

};

} // namespace gfx::detail
