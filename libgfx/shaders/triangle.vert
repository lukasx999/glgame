#version 330 core

in int a_vertex_no;
uniform mat4 u_projection;
uniform vec2 u_position0;
uniform vec2 u_position1;
uniform vec2 u_position2;

vec2 get_position(int vertex_no) {
    switch (a_vertex_no) {
        case 0: return u_position0;
        case 1: return u_position1;
        case 2: return u_position2;
    }
}

void main() {
    gl_Position = u_projection * vec4(get_position(a_vertex_no), 0.0f, 1.0f);
}
