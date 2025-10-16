#version 330 core

out vec4 FragColor;

in VSOut {
    vec4 color;
} vs_out;

void main() {
    FragColor = vs_out.color;
}
