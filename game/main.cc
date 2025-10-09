#include <print>
#include <string>
#include <fstream>

#include "gfx.hh"

using gfx::Color;
using gfx::KeyState;

namespace {

void handle_inputs(gfx::Renderer& rd) {
    if (rd.get_window().get_key_state(gfx::Key::Escape) == KeyState::Press) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}

void render_grid(gfx::Renderer& rd, int n, int size, gfx::Color color) {
    for (int x=0; x < n; ++x) {
        for (int y=0; y < n; ++y) {
            float factor = 0.1;
            rd.draw_rectangle(x*(size+size*factor), y*(size+size*factor), size, size, 0, color);
        }
    }
}

void render_grid_textured(gfx::Renderer& rd, int n, int size, const gfx::Texture& texture) {
    for (int x=0; x < n; ++x) {
        for (int y=0; y < n; ++y) {
            float factor = 0.1;
            rd.draw_texture(x*(size+size*factor), y*(size+size*factor), size, size, 0, texture);
        }
    }
}

void render_grid_triangles(gfx::Renderer& rd, int n, int size, gfx::Color color) {
    for (int x=0; x < n; ++x) {
        for (int y=0; y < n; ++y) {
            float factor = 0.1;
            rd.draw_triangle(
                x*(size+size*factor),
                y*(size+size*factor),
                x*(size+size*factor)+size,
                y*(size+size*factor),
                x*(size+size*factor),
                y*(size+size*factor)+size,
                color
            );
        }
    }
}

} // namespace

int main() {

    gfx::Window window(600, 600, "GLGame", false);
    gfx::Renderer rd(window);

    gfx::Texture tex("./awesomeface.png");

    while (!window.should_close()) {
        rd.with_draw_context([&]() {

            rd.clear_background(Color::black());
            std::println("fps: {}", std::round(rd.get_fps()));

            // render_grid(rd, 300, 5, 0x595959ff);
            render_grid_textured(rd, 300, 50, tex);

            handle_inputs(rd);
        });

    }

}
