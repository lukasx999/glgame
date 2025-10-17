#pragma once

#include <print>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <types.hh>

namespace gfx {

class Window {
    friend class Renderer;
    GLFWwindow* m_window;

public:
    Window(int width, int height, const char* window_title, bool resizable_window)
    : m_window(init_glfw(width, height, window_title, resizable_window))
    { }

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    ~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    [[nodiscard]] bool should_close() const {
        return glfwWindowShouldClose(m_window);
    }

    void close() {
        glfwSetWindowShouldClose(m_window, true);
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

    [[nodiscard]] double get_time() const {
        return glfwGetTime();
    }

    [[nodiscard]] KeyState get_key_state(Key key) const {
        return KeyState(glfwGetKey(m_window, gfx_key_to_glfw_key(key)));
    }

private:
    [[nodiscard]] static GLFWwindow* init_glfw(int width, int height, const char* window_title, bool resizable_window);
    [[nodiscard]] static int gfx_key_to_glfw_key(Key key);

    static void debug_message_callback(
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar *message,
        [[maybe_unused]] const void *user_param
    ) {

        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
        std::println(stderr, "opengl error: {}", message);
    }

};

} // namespace gfx
