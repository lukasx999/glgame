#include <renderer.hh>

namespace gfx {

void Renderer::with_draw_context(std::function<void()> draw_fn) {
    calculate_frame_time();
    glViewport(0, 0, m_window.get_width(), m_window.get_height());
    draw_fn();
    flush();
    glfwSwapBuffers(m_window.m_window);
    glfwPollEvents();
}

} // namespace gfx
