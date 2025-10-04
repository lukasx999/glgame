#pragma once

namespace {

inline constexpr char shader_vertex_src[] = {
    #embed "../shaders/default.vert" suffix(,)
    '\0'
};

inline constexpr char shader_fragment_src[] = {
    #embed "../shaders/default.frag" suffix(,)
    '\0'
};

inline constexpr char circle_shader_fragment_src[] = {
    #embed "../shaders/circle.frag" suffix(,)
    '\0'
};

} // namespace
