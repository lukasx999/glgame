#version 330 core

in vec2 a_pos;
in vec2 a_uv;
out vec2 frag_uv;
uniform mat4 u_mvp;

void main() {
    gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);
    frag_uv = a_uv;
}
