#pragma once

namespace {

inline constexpr char shader_vertex_src[] = {
    #embed "../shaders/default.vert"
};

inline constexpr char shader_fragment_src[] = {
    #embed "../shaders/default.frag"
};

inline constexpr char circle_shader_fragment_src[] = {
    #embed "../shaders/circle.frag"
};

} // namespace
