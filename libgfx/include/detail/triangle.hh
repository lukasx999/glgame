#pragma once

#include <array>

#include <window.hh>
#include <types.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class TriangleRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;

public:
    explicit TriangleRenderer(gfx::Window& window);

    void draw(int x0, int y0, int x1, int y1, int x2, int y2, gfx::Color color);

};

} // namespace gfx::detail
