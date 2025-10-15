// THIS FILE IS GENERATED

#pragma once

namespace shaders {

namespace vertex {

inline constexpr char default_[] = {
#embed "../shaders/default.vert" suffix(,)
'\0'
};

inline constexpr char texture[] = {
#embed "../shaders/texture.vert" suffix(,)
'\0'
};

inline constexpr char text[] = {
#embed "../shaders/text.vert" suffix(,)
'\0'
};

inline constexpr char batched[] = {
#embed "../shaders/batched.vert" suffix(,)
'\0'
};

} // namespace vertex

namespace fragment {

inline constexpr char text[] = {
#embed "../shaders/text.frag" suffix(,)
'\0'
};

inline constexpr char default_[] = {
#embed "../shaders/default.frag" suffix(,)
'\0'
};

inline constexpr char batched[] = {
#embed "../shaders/batched.frag" suffix(,)
'\0'
};

inline constexpr char texture[] = {
#embed "../shaders/texture.frag" suffix(,)
'\0'
};

inline constexpr char circle[] = {
#embed "../shaders/circle.frag" suffix(,)
'\0'
};

} // namespace fragment

} // namespace shaders
