#pragma once

#include <cmath>
#include <cstdint>

namespace gfx {

[[nodiscard]] inline constexpr float deg_to_rad(float deg) {
    return deg * (M_PI / 180.0);
}

[[nodiscard]] inline constexpr float rad_to_deg(float rad) {
    return rad * (180.0 / M_PI);
}

class IRotation {
public:
    [[nodiscard]] virtual float get_radians() const = 0;
    [[nodiscard]] virtual float get_degrees() const = 0;
    virtual ~IRotation() = default;
};

class Radians : public IRotation {
    float m_radians;

public:
    explicit constexpr Radians(float radians) : m_radians(radians) { }

    [[nodiscard]] constexpr float get_radians() const override {
        return m_radians;
    }

    [[nodiscard]] constexpr float get_degrees() const override {
        return rad_to_deg(m_radians);
    }

};

class Degrees : public IRotation {
    float m_degrees;

public:
    explicit constexpr Degrees(float degrees) : m_degrees(degrees) { }

    [[nodiscard]] constexpr float get_radians() const override {
        return deg_to_rad(m_degrees);
    }

    [[nodiscard]] constexpr float get_degrees() const override {
        return m_degrees;
    }

};

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

    [[nodiscard]] constexpr auto normalized() const {
        struct NormalizedColor {
            float r, g, b, a;
        };

        return NormalizedColor {
            r / 255.0f,
            g / 255.0f,
            b / 255.0f,
            a / 255.0f
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

enum class Key {
    Escape,
    Space,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
};

} // namespace gfx

inline consteval gfx::Degrees operator""_deg(unsigned long long value) {
    return gfx::Degrees(value);
}

inline consteval gfx::Radians operator""_rad(unsigned long long value) {
    return gfx::Radians(value);
}
