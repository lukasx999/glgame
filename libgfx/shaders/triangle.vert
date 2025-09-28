#version 330 core

in vec2 a_position;
in int a_vertex_no;
uniform mat4 u_projection;
// uniform vec2 u_position0;
// uniform vec2 u_position1;
// uniform vec2 u_position2;
uniform mat4 u_model0;
uniform mat4 u_model1;
uniform mat4 u_model2;


mat4 get_model(int vertex_no) {
    switch (a_vertex_no) {
        case 0: return u_model0;
        case 1: return u_model1;
        case 2: return u_model2;
    }
}

// vec2 get_position(int vertex_no) {
//     switch (a_vertex_no) {
//         case 0: return u_position0;
//         case 1: return u_position1;
//         case 2: return u_position2;
//     }
// }

void main() {
    // gl_Position = u_projection * vec4(get_position(a_vertex_no), 0.0f, 1.0f);
    gl_Position = u_projection * get_model(a_vertex_no) * vec4(a_position, 0.0f, 1.0f);
}
