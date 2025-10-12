#version 330 core

out vec4 FragColor;
in vec2 frag_uv;
uniform sampler2D tex;

void main() {
    FragColor = vec4(texture(tex, frag_uv).r, 0.0, 0.0, 1.0);
}
