#include <print>
#include <string>
#include <fstream>

#include "gfx.hh"

using gfx::Vec;
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
            rd.draw_rectangle(x*(size+size*factor), y*(size+size*factor), size, size, 0_deg, color);
        }
    }
}

void render_grid_textured(gfx::Renderer& rd, int n, int size, const gfx::Texture& texture) {
    for (int x=0; x < n; ++x) {
        for (int y=0; y < n; ++y) {
            float factor = 0.1;
            rd.draw_texture(x*(size+size*factor), y*(size+size*factor), size, size, 0_deg, texture);
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

void draw_spiked_ball(gfx::Renderer& rd, float step) {
    int height = rd.get_window().get_height();
    int width = rd.get_window().get_width();

    for (float angle = 0; angle <= 360; angle += step) {
        Vec midpoint(width/2.0f, height/2.0f);
        Vec pos(0, 200);

        pos.rotate(gfx::Degrees(angle));

        Vec end = midpoint + pos;
        rd.draw_line(midpoint.x, midpoint.y, end.x, end.y, gfx::Color::red());
    }
}

} // namespace

int main() {

    gfx::Window window(600, 600, "GLGame", false);
    gfx::Renderer rd(window);

    // gfx::Texture tex("./awesomeface.png");

    while (!window.should_close()) {
        rd.with_draw_context([&]() {

            rd.clear_background(Color::black());
            std::println("fps: {}", std::round(rd.get_fps()));

            draw_spiked_ball(rd, 0.001);

            // render_grid(rd, 300, 5, 0x595959ff);
            // render_grid_textured(rd, 300, 50, tex);

            handle_inputs(rd);
        });

    }

}
