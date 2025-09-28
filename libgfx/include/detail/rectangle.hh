#pragma once

#include <array>

#include <types.hh>
#include <detail/detail.hh>

namespace detail {

class RectangleRenderer {
    GLFWwindow* m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_index_buffer;

    static constexpr std::array m_vertices {
        Vertex({ 1.0, 1.0 }), // top-right
        Vertex({ 0.0, 1.0 }), // top-left
        Vertex({ 0.0, 0.0 }), // bottom-left
        Vertex({ 1.0, 0.0 }), // bottom-right
    };

    static constexpr std::array m_indices {
        0u, 1u, 2u,
        3u, 2u, 0u,
    };

public:
    explicit RectangleRenderer(GLFWwindow* window);

    void draw(int x, int y, int width, int height, float rotation, gfx::Color color) const;

};

} // namespace detail
