#pragma once

#include <functional>
#include <stdexcept>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/rectangle.hh>
#include <detail/triangle.hh>
#include <types.hh>

namespace gfx {

class Gfx {
    GLFWwindow* m_window;
    detail::RectangleRenderer m_rectangle;
    detail::TriangleRenderer m_triangle;

public:
    Gfx(int width, int height, const char* window_title, bool resizable_window)
        : m_window(init_glfw(width, height, window_title, resizable_window))
        , m_rectangle(m_window)
        , m_triangle(m_window)
    { }

    ~Gfx() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void with_draw_context(std::function<void()> draw_fn) {
        glViewport(0, 0, get_width(), get_height());
        draw_fn();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    [[nodiscard]] bool window_should_close() const {
        return glfwWindowShouldClose(m_window);
    }

    [[nodiscard]] int get_width() {
        int width;
        glfwGetFramebufferSize(m_window, &width, nullptr);
        return width;
    }

    [[nodiscard]] int get_height() {
        int height;
        glfwGetFramebufferSize(m_window, nullptr, &height);
        return height;
    }

    [[nodiscard]] KeyState get_key_state(int key) const {
        // TODO: python script to generate wrappers for glfw keys

        int state = glfwGetKey(m_window, key);
        switch (state) {
            case GLFW_RELEASE: return KeyState::Release;
            case GLFW_PRESS:   return KeyState::Press;
            case GLFW_REPEAT:  return KeyState::Repeat;

        }
        throw std::runtime_error("unknown glfw key state");
    }

    // TODO: draw_texture()
    // TODO: draw_circle()

    void draw_rectangle(int x, int y, int width, int height, Color color) {
        m_rectangle.draw(x, y, width, height, color);
    }

    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color) {
        m_triangle.draw(x0, y0, x1, y1, x2, y2, color);
    }

    void clear_background(Color color) {
        auto normalized = color.normalized();
        glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

private:
    [[nodiscard]] static GLFWwindow* init_glfw(int width, int height, const char* window_title, bool resizable_window);
};

} // namespace gfx
