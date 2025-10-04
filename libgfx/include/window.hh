#pragma once

#include <stdexcept>
#include <functional>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <types.hh>

namespace gfx {

class Window {
    GLFWwindow* m_window;

public:
    Window(int width, int height, const char* window_title, bool resizable_window)
    : m_window(init_glfw(width, height, window_title, resizable_window))
    { }

    ~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    [[nodiscard]] bool should_close() const {
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

    void with_draw_context(std::function<void()> draw_fn) {
        glViewport(0, 0, get_width(), get_height());
        draw_fn();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
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

private:
    [[nodiscard]] static GLFWwindow* init_glfw(int width, int height, const char* window_title, bool resizable_window);

};

} // namespace gfx
