#pragma once

#include <array>
#include <vector>

#include <window.hh>
#include <types.hh>
#include <detail/detail.hh>

namespace detail {

class RectangleRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_transform_buffer;
    GLuint m_index_buffer;

    struct InstanceData {
        glm::mat4 mvp;
        glm::vec4 color;
    };

    std::vector<InstanceData> m_instance_data;

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
    explicit RectangleRenderer(gfx::Window& window);
    void draw(int x, int y, int width, int height, const gfx::IRotation& rotation, gfx::Color color);
    void flush();

};

} // namespace detail
