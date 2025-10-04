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

    [[nodiscard]] static constexpr int gfx_key_to_glfw_key(Key key) {
        switch (key) {
            using enum Key;

            case Escape: return GLFW_KEY_ESCAPE;
            case Space: return GLFW_KEY_SPACE;
            case Num0: return GLFW_KEY_0;
            case Num1: return GLFW_KEY_1;
            case Num2: return GLFW_KEY_2;
            case Num3: return GLFW_KEY_3;
            case Num4: return GLFW_KEY_4;
            case Num5: return GLFW_KEY_5;
            case Num6: return GLFW_KEY_6;
            case Num7: return GLFW_KEY_7;
            case Num8: return GLFW_KEY_8;
            case Num9: return GLFW_KEY_9;
            case A: return GLFW_KEY_A;
            case B: return GLFW_KEY_B;
            case C: return GLFW_KEY_C;
            case D: return GLFW_KEY_D;
            case E: return GLFW_KEY_E;
            case F: return GLFW_KEY_F;
            case G: return GLFW_KEY_G;
            case H: return GLFW_KEY_H;
            case I: return GLFW_KEY_I;
            case J: return GLFW_KEY_J;
            case K: return GLFW_KEY_K;
            case L: return GLFW_KEY_L;
            case M: return GLFW_KEY_M;
            case N: return GLFW_KEY_N;
            case O: return GLFW_KEY_O;
            case P: return GLFW_KEY_P;
            case Q: return GLFW_KEY_Q;
            case R: return GLFW_KEY_R;
            case S: return GLFW_KEY_S;
            case T: return GLFW_KEY_T;
            case U: return GLFW_KEY_U;
            case V: return GLFW_KEY_V;
            case W: return GLFW_KEY_W;
            case X: return GLFW_KEY_X;
            case Y: return GLFW_KEY_Y;
            case Z: return GLFW_KEY_Z;
        }
    }

};

} // namespace gfx
