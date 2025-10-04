#include <print>

#include "window.hh"

GLFWwindow* gfx::Window::init_glfw(int width, int height, const char* window_title, bool resizable_window) {

    glfwSetErrorCallback([]([[maybe_unused]] int error, const char* desc) {
        std::println(stderr, "glfw error: {}", desc);
    });

    if (!glfwInit())
        throw std::runtime_error("failed to create window");

    glfwWindowHint(GLFW_RESIZABLE, resizable_window);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    auto window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    // TODO: #define GFX_WIREFRAME macro for toggling this
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
