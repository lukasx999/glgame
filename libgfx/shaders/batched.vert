#version 330 core

in vec2 a_pos;
in vec4 a_color;
out vec4 frag_color;

void main() {
    gl_Position = vec4(a_pos, 0.0, 1.0);
    frag_color = a_color;
}
