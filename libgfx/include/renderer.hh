#pragma once

#include <array>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/rectangle.hh>
#include <detail/circle.hh>
#include <detail/triangle.hh>
#include <detail/texture.hh>
#include <detail/line.hh>
#include <detail/text.hh>

#include <types.hh>
#include <vec.hh>
#include <window.hh>

// TODO: auto-invoke python script for generating shader glue code
// TODO: 2d camera
// TODO: tui for statistics
// TODO: show average fps
// TODO: limit fps eg: set_fps(std::optional<int> fps)
// TODO: rotation for textures/rectangles
// TODO: circle using GL_POINTS
// TODO: fix glfw resizing window
// TODO: anti-aliasing
// TODO: overloads for draw functions with gfx::Vec

namespace gfx {

class Renderer {
    Window& m_window;

    detail::RectangleRenderer m_rectangle;
    detail::CircleRenderer m_circle;
    detail::TriangleRenderer m_triangle;
    detail::TextureRenderer m_texture;
    detail::LineRenderer m_line;
    detail::TextRenderer m_text;

    std::array<std::reference_wrapper<detail::IBatchRenderer>, 4> m_batch_renderers {
        m_rectangle,
        m_triangle,
        m_texture,
        m_line
    };

    double m_frame_time = 0.0;
    double m_last_frame = 0.0;

    glm::mat4 m_view_default = gen_view_matrix(m_window, m_window.get_width()/2, m_window.get_height()/2);
    glm::mat4 m_view_camera = m_view_default;
    glm::mat4 m_view_active = m_view_default;

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

    void with_camera(std::function<void()> draw_fn) {
        m_view_active = m_view_camera;
        draw_fn();
        m_view_active = m_view_default;
    }

    // TODO: rotation
    void set_camera(int center_x, int center_y) {
        m_view_camera = gen_view_matrix(m_window, center_x, center_y);
    }

    void set_camera(gfx::Vec vec) {
        set_camera(vec.x, vec.y);
    }

    void draw_rectangle(
        int x,
        int y,
        int width,
        int height,
        const gfx::IRotation& rotation,
        gfx::Color color
    ) {
        flush_all_except(m_rectangle);
        m_rectangle.draw(x, y, width, height, rotation, color, m_view_active);
    }

    void draw_texture(
        int x,
        int y,
        int width,
        int height,
        const gfx::IRotation& rotation,
        const gfx::Texture& texture
    ) {
        flush_all_except(m_texture);
        m_texture.draw(x, y, width, height, rotation, texture, m_view_active);
    }

    void draw_circle(int x, int y, int radius, gfx::Color color) {
        flush();
        m_circle.draw(x, y, radius, color);
    }

    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, gfx::Color color) {
        flush_all_except(m_triangle);
        m_triangle.draw(x0, y0, x1, y1, x2, y2, color, m_view_active);
    }

    void draw_line(int x0, int y0, int x1, int y1, gfx::Color color) {
        flush_all_except(m_line);
        m_line.draw(x0, y0, x1, y1, color, m_view_active);
    }

    void draw_text(int x, int y, int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
        flush();
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
        for (auto& rd : m_batch_renderers) {
            rd.get().flush();
        }
    }

    // make sure all other shapes have already been drawn to the screen,
    // as otherwise the drawing order will be incorrect, leading weird
    // overlapping shapes
    //
    // therefore we have to flush all other shapes before drawing the current one
    //
    // this has the unfortunate effect of breaking batching optimizations
    // when using interleaved rendering when drawing a lot of shapes
    void flush_all_except(const detail::IBatchRenderer& exception) {
        for (auto& rd : m_batch_renderers) {
            if (reinterpret_cast<detail::IBatchRenderer*>(&rd) == &exception)
                continue;
            rd.get().flush();
        }
    }

    void calculate_frame_time() {
        double time = glfwGetTime();
        m_frame_time = time - m_last_frame;
        m_last_frame = time;
    }

    [[nodiscard]] static constexpr
    glm::mat4 gen_view_matrix(const Window& window, int center_x, int center_y) {
        glm::vec3 camera_position(
            center_x - window.get_width() / 2.0f,
            center_y - window.get_height() / 2.0f,
            0.0f
        );
        glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::mat4 view = glm::lookAt(camera_position, camera_position+camera_direction, up);
        return view;
    }

};

} // namespace gfx
