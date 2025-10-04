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
    double m_frame_time = 0.0;
    double m_last_frame = 0.0;

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

    [[nodiscard]] int get_width() const {
        int width;
        glfwGetFramebufferSize(m_window, &width, nullptr);
        return width;
    }

    [[nodiscard]] int get_height() const {
        int height;
        glfwGetFramebufferSize(m_window, nullptr, &height);
        return height;
    }

    void with_draw_context(std::function<void()> draw_fn) {
        calculate_frame_time();

        glViewport(0, 0, get_width(), get_height());
        draw_fn();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    [[nodiscard]] double get_frame_time() const {
        return m_frame_time;
    }

    [[nodiscard]] double get_fps() const {
        return 1.0 / m_frame_time;
    }

    [[nodiscard]] KeyState get_key_state(Key key) const {
        int state = glfwGetKey(m_window, gfx_key_to_glfw_key(key));

        switch (state) {
            case GLFW_RELEASE: return KeyState::Release;
            case GLFW_PRESS:   return KeyState::Press;
            case GLFW_REPEAT:  return KeyState::Repeat;

        }

        throw std::runtime_error("unknown glfw key state");
    }

private:
    [[nodiscard]] static GLFWwindow* init_glfw(int width, int height, const char* window_title, bool resizable_window);
    [[nodiscard]] static int gfx_key_to_glfw_key(Key key);

    void calculate_frame_time() {
        double time = glfwGetTime();
        m_frame_time = time - m_last_frame;
        m_last_frame = time;
    }

};

} // namespace gfx
