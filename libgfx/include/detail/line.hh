#pragma once

#include <vector>

#include <window.hh>
#include <types.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class LineRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_color_buffer;
    GLuint m_vertex_buffer;

    // colors are stored in an instanced array
    std::vector<glm::vec4> m_colors;
    // accumulator used for batch rendering
    std::vector<Vertex> m_batch;

public:
    explicit LineRenderer(gfx::Window& window);
    void draw(int x0, int y0, int x1, int y1, gfx::Color color);
    void flush();

};

} // namespace gfx::detail
