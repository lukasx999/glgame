#include <gfx.hh>

int main() {

    gfx::Window window(600, 600, "GLGame", false);
    gfx::Renderer rd(window);

    rd.draw([&]() {
        rd.clear_background(gfx::Color::black());
        rd.draw_rectangle(0, 0, 100, 100, 0_deg, gfx::Color::red());
    });

}
