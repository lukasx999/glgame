#pragma once

namespace {

inline constexpr char shader_vertex_src[] = {
    #embed "../shaders/shader.vert"
};

inline constexpr char shader_fragment_src[] = {
    #embed "../shaders/shader.frag"
};

inline constexpr char triangle_shader_vertex_src[] = {
    #embed "../shaders/triangle.vert"
};

inline constexpr char circle_shader_fragment_src[] = {
    #embed "../shaders/circle.frag"
};

} // namespace
