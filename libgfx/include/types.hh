#pragma once

#include <cmath>
#include <filesystem>
#include <cstdint>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace gfx {

namespace detail {
class TextRenderer;
} // namespace detail

// TODO: should probably be private
struct Glyph {
    unsigned int width;
    unsigned int height;
    int bearing_x;
    int bearing_y;
    unsigned int advance;
    unsigned char* buffer;
};

// TODO: text measurement
class Font {
    friend detail::TextRenderer;
    FT_Face m_face;

    Font(FT_Library ft, const char* path) {
        if (FT_New_Face(ft, path, 0, &m_face) != 0) {
            throw std::runtime_error("failed to load font");
        }
    }

public:
    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&) = delete;

    ~Font() {
        FT_Done_Face(m_face);
    }

    [[nodiscard]] Glyph load_glyph(char c, int size) const {

        if (FT_Set_Pixel_Sizes(m_face, 0, size)) {
            throw std::runtime_error("failed to set pixel size");
        }

        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER) != 0) {
            throw std::runtime_error("failed to load char");
        }

        unsigned int width = m_face->glyph->bitmap.width;
        unsigned int height = m_face->glyph->bitmap.rows;
        int bearing_x = m_face->glyph->bitmap_left;
        int bearing_y = m_face->glyph->bitmap_top;
        unsigned int advance = m_face->glyph->advance.x;
        unsigned char* buffer = m_face->glyph->bitmap.buffer;

        return Glyph {
            width,
            height,
            bearing_x,
            bearing_y,
            // advance is pixels * 64
            advance / 64,
            buffer,
        };
    }

};

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

class Texture {
    int m_width;
    int m_height;
    int m_channels;
    unsigned char* m_data;
    // TODO: move opengl texture loading into here

public:
    Texture(const char* path);
    ~Texture();

    [[nodiscard]] unsigned char* get_data() const {
        return m_data;
    }

    [[nodiscard]] int get_width() const {
        return m_width;
    }

    [[nodiscard]] int get_height() const {
        return m_height;
    }

    [[nodiscard]] int get_channels() const {
        return m_channels;
    }

    // TODO: copy/move ctor
    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

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
