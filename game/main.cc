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

} // namespace

int main() {

    gfx::Window window(1920, 1080, "GLGame", false);
    gfx::Renderer rd(window);

    while (!window.should_close()) {
        window.with_draw_context([&]() {

            rd.clear_background(Color::black());
            std::println("fps: {}", window.get_fps());

            render_grid(rd, 100, 15, 0x595959ff);

            // rd.draw_rectangle(1000, 500, 100, 100, 0, Color::red());
            // rd.draw_rectangle(0, 0, 300, 100, 0, Color::blue());
            // rd.draw_rectangle(0, 100, 100, 100, 0, Color::white());
            // auto x = (std::sin(glfwGetTime()*3) + 1) / 2;
            //
            // int width = 300;
            // int height = 100;
            // int anim_x = x*(1920-width);
            // int y = 500;
            // rd.draw_rectangle(anim_x, y, width, height, 0, Color::green());
            // rd.draw_rectangle(anim_x, y, width, height, x*360, Color::blue());
            //
            // rd.draw_circle(anim_x+width/2.0f, y+height/2, 50, Color::red());
            // rd.draw_circle(300, 300, 150, Color::green());
            // rd.draw_circle(300, 300, x*150, Color::blue());

            handle_inputs(rd);
        });

    }

}
