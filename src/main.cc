#include <print>
#include <string>
#include <fstream>

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

void handle_inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}

[[nodiscard]] GLFWwindow* init_glfw(int width, int height, const char* window_title) {

    glfwSetErrorCallback([]([[maybe_unused]] int error, const char* desc) {
        std::println(stderr, "glfw error: {}", desc);
    });

    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    auto window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return nullptr;
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
    ) { std::println(stderr, "opengl error: {}", message); }, nullptr);

    return window;
}

} // namespace

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    Color() = default;

    Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
        : r(r_)
        , g(g_)
        , b(b_)
        , a(a_)
    { }

    Color(uint32_t color)
        : r(color >> 8*3 & 0xff)
        , g(color >> 8*2 & 0xff)
        , b(color >> 8*1 & 0xff)
        , a(color >> 8*0 & 0xff)
    { }

    [[nodiscard]] constexpr Color normalized() const {
        return {
            static_cast<uint8_t>(r / 0xff),
            static_cast<uint8_t>(g / 0xff),
            static_cast<uint8_t>(b / 0xff),
            static_cast<uint8_t>(a / 0xff)
        };
    }

    [[nodiscard]] static constexpr Color red() {
        return 0xff0000ff;
    }

    [[nodiscard]] static constexpr Color green() {
        return 0x00ff00ff;
    }

    [[nodiscard]] static constexpr Color blue() {
        return 0x0000ffff;
    }

};

class RectangleRenderer {
    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_index_buffer;

    static constexpr std::array m_vertices {
        Vertex({ 1.0, 1.0 }), // top-right
        Vertex({ 0.0, 1.0 }), // top-left
        Vertex({ 0.0, 0.0 }), // bottom-left
        Vertex({ 1.0, 0.0 }), // bottom-right
    };

    static constexpr std::array m_indices {
        0u, 1u, 2u,
        3u, 2u, 0u,
    };

public:
    RectangleRenderer() {
        glGenVertexArrays(1, &m_vertex_array);
        glBindVertexArray(m_vertex_array);

        glGenBuffers(1, &m_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &m_index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

        m_program = create_shader_program();

        GLint a_pos = glGetAttribLocation(m_program, "a_pos");
        glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_position)));
        glEnableVertexAttribArray(a_pos);

        // just to make sure everything still works after unbinding, as other classes/functions may
        // modify opengl state after running the ctor
        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw_rectangle(int x, int y, int width, int height, Color color) const {

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(x, y, 0.0f));
        model = glm::scale(model, glm::vec3(width, height, 0.0f));

        glm::mat4 view(1.0f);
        glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f);
        glm::mat4 mvp = projection * view * model;

        glUseProgram(m_program);

        GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
        glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

        GLint u_color = glGetUniformLocation(m_program, "u_color");
        auto normalized = color.normalized();
        glUniform4f(u_color, normalized.r, normalized.g, normalized.b, normalized.a);

        glBindVertexArray(m_vertex_array);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }

private:
    [[nodiscard]] static GLuint create_shader_program() {

        GLuint vertex_shader = load_shader("shader.vert", GL_VERTEX_SHADER);
        GLuint fragment_shader = load_shader("shader.frag", GL_FRAGMENT_SHADER);
        GLuint program = glCreateProgram();

        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);

        glLinkProgram(program);
        glUseProgram(program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }

    [[nodiscard]] static GLuint load_shader(const char* path, GLenum type) {
        GLuint vertex_shader = glCreateShader(type);

        auto file = read_entire_file(path);
        auto src = file.c_str();

        glShaderSource(vertex_shader, 1, &src, nullptr);
        glCompileShader(vertex_shader);

        return vertex_shader;
    }


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

    auto window = init_glfw(1920, 1080, "GLGame");
    if (window == nullptr)
        return EXIT_FAILURE;

    RectangleRenderer renderer;

    // TODO:
    // clear_background()
    // glClearColor();
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.draw_rectangle(1000, 500, 100, 100, Color::red());
        renderer.draw_rectangle(0, 0, 300, 100, Color::blue());

        handle_inputs(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
