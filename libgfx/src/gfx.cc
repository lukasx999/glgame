#include <print>

// has to be included before defining GLAD_GL_IMPLEMENTATION, as we'd otherwise
// get 2 implementations
#include "gfx.hh"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

GLFWwindow* gfx::Gfx::init_glfw(int width, int height, const char* window_title, bool resizable_window) {

    glfwSetErrorCallback([]([[maybe_unused]] int error, const char* desc) {
        std::println(stderr, "glfw error: {}", desc);
    });

    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_RESIZABLE, resizable_window);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    auto window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDebugMessageCallback([](
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        [[maybe_unused]] GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar *message,
        [[maybe_unused]] const void *user_param
    ) { std::println(stderr, "opengl error: {}", message); }, nullptr);

    return window;
}
