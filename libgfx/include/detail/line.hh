#pragma once

#include <vector>

#include <window.hh>
#include <types.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class LineRenderer : public IBatchRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_color_buffer;
    GLuint m_vertex_buffer;
    GLuint m_transform_buffer;

    std::vector<glm::vec4> m_colors;
    std::vector<glm::vec2> m_vertices;
    std::vector<glm::mat4> m_transforms;

public:
    explicit LineRenderer(gfx::Window& window);
    void draw(float x0, float y0, float x1, float y1, gfx::Color color, glm::mat4 view);
    void flush() override;

};

} // namespace gfx::detail
