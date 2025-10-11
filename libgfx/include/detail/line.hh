#pragma once

#include <array>
#include <vector>

#include <window.hh>
#include <types.hh>
#include <detail/detail.hh>

namespace detail {

class LineRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;

public:
    explicit LineRenderer(gfx::Window& window);
    void draw(int x0, int y0, int x1, int y1, gfx::Color color);

};

} // namespace detail
