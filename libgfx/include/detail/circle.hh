#pragma once

#include <array>

#include <types.hh>
#include <detail/detail.hh>

namespace detail {

class CircleRenderer {
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
    explicit CircleRenderer(GLFWwindow* window);

    // TODO: is this useful?
    // template <typename... Args>
    // static void set_uniform(GLuint program, const char* name, std::function<void(Args...)> fn, Args&& ...args) {
    //     GLint location = glGetUniformLocation(program, name);
    //     fn(location, std::forward<Args>(args)...);
    // }

    void draw(int x, int y, int radius, gfx::Color color);

};

} // namespace detail
