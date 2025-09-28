#version 330 core

out vec4 FragColor;
uniform vec4 u_color;
uniform int u_radius;
uniform vec2 u_center;
uniform int u_window_height;

// circles are rendered as squares, where all fragments outside of the circle's radius
// are discarded
void main() {
    vec2 coord = gl_FragCoord.xy;

    // gl_FragCoord's origin is at the bottom left
    coord.y = float(u_window_height) - coord.y;

    if (distance(coord, u_center) > u_radius) {
        discard;
    }

    FragColor = u_color;
}
