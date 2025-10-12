#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/rectangle.hh>
#include <detail/circle.hh>
#include <detail/triangle.hh>
#include <detail/texture.hh>
#include <detail/line.hh>
#include <detail/text.hh>

#include <types.hh>
#include <window.hh>

// TODO: auto-invoke python script for generating shader glue code
// TODO: 2d camera
// TODO: tui for statistics
// TODO: limit fps eg: set_fps(std::optional<int> fps)
// TODO: draw_text() via freetype

namespace gfx {

class Renderer {
    Window& m_window;

    detail::RectangleRenderer m_rectangle;
    detail::CircleRenderer m_circle;
    detail::TriangleRenderer m_triangle;
    detail::TextureRenderer m_texture;
    detail::LineRenderer m_line;
    detail::TextRenderer m_text;

    double m_frame_time = 0.0;
    double m_last_frame = 0.0;

public:
    explicit Renderer(Window& window)
        : m_window(window)
        , m_rectangle(m_window)
        , m_circle(m_window)
        , m_triangle(m_window)
        , m_texture(m_window)
        , m_line(m_window)
        , m_text(m_window)
    { }

    [[nodiscard]] Window& get_window() const {
        return m_window;
    }

    void with_draw_context(std::function<void()> draw_fn);

    [[nodiscard]] double get_frame_time() const {
        return m_frame_time;
    }

    [[nodiscard]] double get_fps() const {
        return 1.0 / m_frame_time;
    }

    void draw_rectangle(int x, int y, int width, int height, const gfx::IRotation& rotation, gfx::Color color) {
        m_rectangle.draw(x, y, width, height, rotation, color);
    }

    void draw_texture(int x, int y, int width, int height, const gfx::IRotation& rotation, const gfx::Texture& texture) {
        m_texture.draw(x, y, width, height, rotation, texture);
    }

    void draw_circle(int x, int y, int radius, gfx::Color color) {
        m_circle.draw(x, y, radius, color);
    }

    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, gfx::Color color) {
        m_triangle.draw(x0, y0, x1, y1, x2, y2, color);
    }

    void draw_line(int x0, int y0, int x1, int y1, gfx::Color color) {
        m_line.draw(x0, y0, x1, y1, color);
    }

    void draw_text(int x, int y, const char* text, const gfx::Font& font) {
        m_text.draw(x, y, text, font);
    }

    // [[nodiscard]] gfx::Font load_font(const char* path) const {
    //     return m_text.load_font(path);
    // }

    void clear_background(Color color) {
        auto normalized = color.normalized();
        glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

private:
    void flush() {
        m_rectangle.flush();
        m_texture.flush();
        m_line.flush();
    }

    void calculate_frame_time() {
        double time = glfwGetTime();
        m_frame_time = time - m_last_frame;
        m_last_frame = time;
    }

};

} // namespace gfx
