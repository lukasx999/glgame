#version 330 core

in vec2 a_pos;
in vec4 a_color;
in mat4 a_mvp;

out VSOut {
    vec4 color;
} vs_out;

void main() {
    gl_Position = a_mvp * vec4(a_pos, 0.0, 1.0);
    vs_out.color = a_color;
}
