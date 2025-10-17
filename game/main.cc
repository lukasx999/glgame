#include <print>
#include <string>
#include <fstream>

#include "gfx.hh"

using gfx::Vec;
using gfx::Color;
using gfx::KeyState;

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

    gfx::Texture tex("./awesomeface.png");
    // gfx::Font font = rd.load_font("/usr/share/fonts/TTF/Roboto-Regular.ttf");
    // gfx::Font font = rd.load_font("/usr/share/fonts/TTF/JetBrainsMono-Regular.ttf");
    gfx::Font font = rd.load_font("/usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf");

    gfx::Vec camera(window.get_width()/2.0f, window.get_height()/2.0f);

    while (!window.should_close()) {
        rd.with_draw_context([&]() {

            rd.clear_background(Color::black());
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

            // draw_spiked_ball(rd, 0.001);


            rd.with_camera([&] {
                rd.draw_rectangle(0, 0, 100, 100, 0_deg, gfx::Color::red());
                rd.draw_rectangle(50, 50, 100, 100, 0_deg, gfx::Color::blue());
                rd.draw_rectangle(100, 100, 100, 100, 0_deg, gfx::Color::green());

                rd.draw_triangle(0, 0, 100, 100, 0, 100, gfx::Color::red());
                rd.draw_triangle(100, 0, 100, 100, 0, 100, gfx::Color::blue());
                rd.draw_texture(0, 0, 100, 100, 45_deg, tex);

            });

            rd.draw_texture(100, 100, 100, 100, 0_deg, tex);

            rd.draw_line(0, 0, 100, 100, gfx::Color::green());
            rd.draw_line(0, 0, 200, 100, gfx::Color::red());
            rd.draw_line(0, 0, 300, 100, gfx::Color::blue());


            // rd.draw_circle(200, 200, 100, gfx::Color::white());


            // rd.draw_text(200, 200, 50, "hellog, world!", font, gfx::Color::red());

            // rd.draw_rectangle(300, 300, 200, 200, 0_deg, gfx::Color::red());
            // rd.draw_rectangle(300, 300, 200, 200, 45_deg, 0x4287f5ff);
            // rd.draw_rectangle(300, 300, 200, 200, 10_deg, gfx::Color::green());

            // render_grid_triangles(rd, 100, 15, gfx::Color::blue());
            // render_grid(rd, 100, 15, gfx::Color::blue());
            // render_grid_textured(rd, 700, 5, tex);

            handle_inputs(rd);
        });

    }

}
