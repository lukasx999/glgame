#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/rectangle.hh>
#include <detail/circle.hh>
#include <types.hh>
#include <window.hh>

// TODO: draw_texture()
// TODO: draw_line()
// TODO: fps measurement

namespace gfx {

class Renderer {
    Window& m_window;
    detail::RectangleRenderer m_rectangle;
    detail::CircleRenderer m_circle;

public:
    explicit Renderer(Window& window)
        : m_window(window)
        , m_rectangle(m_window)
        , m_circle(m_window)
    { }

    [[nodiscard]] Window& get_window() const {
        return m_window;
    }

    void draw_rectangle(int x, int y, int width, int height, float rotation, Color color) {
        m_rectangle.draw(x, y, width, height, rotation, color);
    }

    void draw_circle(int x, int y, int radius, gfx::Color color) {
        m_circle.draw(x, y, radius, color);
    }

    void clear_background(Color color) {
        auto normalized = color.normalized();
        glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

} // namespace gfx
