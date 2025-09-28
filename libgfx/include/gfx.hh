#pragma once

#include <functional>
#include <stdexcept>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/rectangle.hh>
#include <types.hh>

namespace gfx {

class Gfx {
    GLFWwindow* m_window;
    detail::RectangleRenderer m_rect;

public:
    Gfx(int width, int height, const char* window_title, bool resizable_window)
        : m_window(init_glfw(width, height, window_title, resizable_window))
        , m_rect(m_window)
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

    void draw_rectangle(int x, int y, int width, int height, Color color) {
        m_rect.draw_rectangle(x, y, width, height, color);
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
