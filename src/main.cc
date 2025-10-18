#include <print>
#include <string>
#include <fstream>

#include "gfx.hh"

namespace {

void handle_inputs(gfx::Renderer& rd) {
    if (rd.get_window().get_key_state(gfx::Key::Escape).pressed()) {
        rd.get_window().close();
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
        gfx::Vec midpoint(width/2.0f, height/2.0f);
        gfx::Vec pos(0, 200);

        pos.rotate(gfx::Degrees(angle));

        gfx::Vec end = midpoint + pos;
        rd.draw_line(midpoint.x, midpoint.y, end.x, end.y, gfx::Color::red());
    }
}

} // namespace

int main() {

    gfx::Window window(600, 600, "GLGame", gfx::WindowFlags::None);
    gfx::Renderer rd(window);

    gfx::Texture tex("./awesomeface.png");
    // gfx::Font font = rd.load_font("/usr/share/fonts/TTF/Roboto-Regular.ttf");
    // gfx::Font font = rd.load_font("/usr/share/fonts/TTF/JetBrainsMono-Regular.ttf");
    gfx::Font font = rd.load_font("/usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf");

    gfx::Vec camera(window.get_width()/2.0f, window.get_height()/2.0f);

    rd.draw([&]() {

        rd.clear_background(gfx::Color::black());
        std::println("width: {}", window.get_width());
        std::println("height: {}", window.get_height());
        std::println("fps: {}", std::round(rd.get_fps()));

        int step = 5;
        if (window.get_key_state(gfx::Key::W).pressed()) {
            camera.y -= step;
        } else if (window.get_key_state(gfx::Key::S).pressed()) {
            camera.y += step;
        } else if (window.get_key_state(gfx::Key::D).pressed()) {
            camera.x += step;
        } else if (window.get_key_state(gfx::Key::A).pressed()) {
            camera.x -= step;
        }

        rd.set_camera(camera);

        rd.draw_rectangle(0, 0, 100, 100, 0_deg, 0xff0000ff);
        rd.draw_rectangle(50, 50, 100, 100, 0_deg, 0x0000ff7f);

        handle_inputs(rd);
    });

}
