#include <print>
#include <string>
#include <fstream>

#include "gfx.hh"

using gfx::Gfx;
using gfx::Color;
using gfx::KeyState;

namespace {

void handle_inputs(Gfx& ctx) {
    if (ctx.get_key_state(GLFW_KEY_ESCAPE) == KeyState::Press) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}

} // namespace

int main() {

    Gfx ctx(1920, 1080, "GLGame", false);

    while (!ctx.window_should_close()) {
        ctx.with_draw_context([&]() {

            ctx.clear_background(Color::black());
            ctx.draw_rectangle(1000, 500, 100, 100, 0, Color::red());
            ctx.draw_rectangle(0, 0, 300, 100, 0, Color::blue());
            auto x = (std::sin(glfwGetTime()*3) + 1) / 2;

            int width = 300;
            int height = 100;
            int anim_x = x*(1920-width);
            int y = 500;
            ctx.draw_rectangle(anim_x, y, width, height, 0, Color::green());
            ctx.draw_rectangle(anim_x, y, width, height, x*360, Color::blue());

            ctx.draw_circle(anim_x+width/2.0f, y+height/2, 50, Color::red());
            ctx.draw_circle(300, 300, 150, Color::green());
            ctx.draw_circle(300, 300, x*150, Color::blue());
            // ctx.draw_triangle(0, 0, 50, 0, 0, 50, Color::red());
            // ctx.draw_triangle(0, 0, 1, 1, 1, 0, Color::red());

            handle_inputs(ctx);
        });

    }

}
