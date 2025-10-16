#version 330 core

out vec4 FragColor;
in vec2 frag_uv;
uniform sampler2D tex;

void main() {
    if (texture(tex, frag_uv).a == 0.0) discard;
    FragColor = texture(tex, frag_uv);
}
