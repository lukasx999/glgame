#include <print>
#include <string>
#include <fstream>
#include <span>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GL/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace {

struct Vertex {
    glm::vec2 m_position;
};

[[nodiscard]] std::string read_entire_file(const char* path) {
    std::ifstream file(path);
    return { std::istreambuf_iterator(file), {} };
}

[[nodiscard]] GLuint load_shader(const char* path, GLenum type) {
    GLuint vertex_shader = glCreateShader(type);

    auto file = read_entire_file(path);
    auto src = file.c_str();

    glShaderSource(vertex_shader, 1, &src, nullptr);
    glCompileShader(vertex_shader);
    return vertex_shader;
}

void handle_inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}

} // namespace

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    Color() = default;

    Color(uint32_t color)
        : r(color >> 8*3 & 0xff)
        , g(color >> 8*2 & 0xff)
        , b(color >> 8*1 & 0xff)
        , a(color >> 8*0 & 0xff)
    { }

    static Color red() {
        return 0xff0000ff;
    }

    static Color green() {
        return 0x00ff00ff;
    }

    static Color blue() {
        return 0x0000ffff;
    }

};

class Renderer {
public:
    Renderer() = default;

    void draw_rectangle(int x, int y, int width, int height, Color color, GLuint program, GLuint vertex_array, std::span<unsigned int> indices) {

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(x, y, 0.0f));
        model = glm::scale(model, glm::vec3(width, height, 0.0f));

        glm::mat4 view(1.0f);
        glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f);
        glm::mat4 mvp = projection * view * model;

        GLint u_mvp = glGetUniformLocation(program, "u_mvp");
        glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

        GLint u_color = glGetUniformLocation(program, "u_color");
        glUniform4f(
            u_color,
            static_cast<float>(color.r) / 0xff,
            static_cast<float>(color.g) / 0xff,
            static_cast<float>(color.b) / 0xff,
            static_cast<float>(color.a) / 0xff
        );

        glUseProgram(program);
        glBindVertexArray(vertex_array);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }

private:

};

/*
int user_main() {

    Window window(1920, 1080, "hello");
    Renderer renderer(window);

    while (!window.should_close()) {
        renderer.draw_rectangle(0, 0, 100, 100, Color::red());
    }

    return 0;
}
*/

int main() {

    glfwSetErrorCallback([]([[maybe_unused]] int error, const char* desc) {
        std::println(stderr, "glfw error: {}", desc);
    });

    if (!glfwInit()) {
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    auto window = glfwCreateWindow(1920, 1080, "GLGame", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glDebugMessageCallback([](
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        [[maybe_unused]] GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar *message,
        [[maybe_unused]] const void *user_param
    ) {
                           std::println(stderr, "opengl error: {}", message);
                           }, nullptr);

    std::array vertices {
        Vertex({ 1.0, 1.0 }), // top-right
        Vertex({ 0.0, 1.0 }), // top-left
        Vertex({ 0.0, 0.0 }), // bottom-left
        Vertex({ 1.0, 0.0 }), // bottom-right
    };

    std::array indices {
        0u, 1u, 2u,
        3u, 2u, 0u,
    };

    GLuint vertex_shader = load_shader("shader.vert", GL_VERTEX_SHADER);
    GLuint fragment_shader = load_shader("shader.frag", GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glUseProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    GLint a_pos = glGetAttribLocation(program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_position)));
    glEnableVertexAttribArray(a_pos);

    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);

        renderer.draw_rectangle(1000, 500, 100, 100, Color::red(), program, vertex_array, indices);
        renderer.draw_rectangle(0, 0, 300, 100, Color::blue(), program, vertex_array, indices);

        handle_inputs(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
