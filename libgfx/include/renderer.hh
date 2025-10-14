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
// TODO: preserve drawing order of shapes (prematurely flush buffer, if a different shape is drawn)
// TODO: batching vs instancing (just batch everything for reusable code? - instancing only really works for rectangles)

/*

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(x+width/2, y+height/2, 0.0f));
    model = glm::rotate(model, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    // subtract half of width & height, needed for centered rotation
    model = glm::translate(model, glm::vec3(-width/2, -height/2, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 0.0f));

    // TODO: camera
    // glm::vec3 camera_position(0.0f, 0.0f, 0.0f);
    // glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
    // glm::vec3 up(0.0f, 1.0f, 0.0f);
    // glm::mat4 view = glm::lookAt(camera_position, camera_position+camera_direction, up);

    glm::mat4 view(1.0f);

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;
*/

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

    void draw_text(int x, int y, int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
        m_text.draw(x, y, text_size, text, font, color);
    }

    [[nodiscard]] gfx::Font load_font(const char* path) const {
        return m_text.load_font(path);
    }

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
