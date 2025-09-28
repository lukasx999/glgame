#pragma once

#include <cstdint>

namespace gfx {

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    Color() = default;

    Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
        : r(r_)
        , g(g_)
        , b(b_)
        , a(a_)
    { }

    Color(uint32_t color)
        : r(color >> 8*3 & 0xff)
        , g(color >> 8*2 & 0xff)
        , b(color >> 8*1 & 0xff)
        , a(color >> 8*0 & 0xff)
    { }

    [[nodiscard]] constexpr Color normalized() const {
        return {
            static_cast<uint8_t>(r / 0xff),
            static_cast<uint8_t>(g / 0xff),
            static_cast<uint8_t>(b / 0xff),
            static_cast<uint8_t>(a / 0xff)
        };
    }

    [[nodiscard]] static constexpr Color black() {
        return 0x000000ff;
    }

    [[nodiscard]] static constexpr Color white() {
        return 0xffffffff;
    }

    [[nodiscard]] static constexpr Color red() {
        return 0xff0000ff;
    }

    [[nodiscard]] static constexpr Color green() {
        return 0x00ff00ff;
    }

    [[nodiscard]] static constexpr Color blue() {
        return 0x0000ffff;
    }

};

enum class KeyState {
    Release,
    Press,
    Repeat,
};

} // namespace gfx
