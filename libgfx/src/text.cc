#include "shaders.hh"
#include <detail/text.hh>
#include <detail/detail.hh>

namespace gfx::detail {

TextRenderer::TextRenderer(gfx::Window& window)
: m_window(window) {

    m_program = create_shader_program(shaders::vertex::text, shaders::fragment::texture);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);


    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec2), m_vertices.data(), GL_STATIC_DRAW);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_pos);


    glGenBuffers(1, &m_uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), m_uvs.data(), GL_STATIC_DRAW);

    GLint a_uv = glGetAttribLocation(m_program, "a_uv");
    glVertexAttribPointer(a_uv, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_uv);


    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);


    if (FT_Init_FreeType(&m_ft) != 0) {
        throw std::runtime_error("failed to initialize ft2");
    }

}

TextRenderer::~TextRenderer() {
    FT_Done_FreeType(m_ft);
}

void TextRenderer::draw(int x, int y, int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
    int offset = 0;

    for (const char* c = text; *c; ++c) {
        auto glyph = font.load_glyph(*c, text_size);
        draw_char(x+offset, y, glyph, color);
        offset += glyph.advance;
    }

}

// TODO: render text under x,y instead of above?
void TextRenderer::draw_char(int x, int y, const Glyph& glyph, gfx::Color color) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    int width = glyph.width;
    int height = glyph.height;

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(x+glyph.bearing_x, y-glyph.bearing_y, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 0.0f));

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * model;
    GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
    glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

    GLint u_color = glGetUniformLocation(m_program, "u_color");
    auto c = color.normalized();
    glUniform4f(u_color, c.r, c.g, c.b, c.a);

    glBindTexture(GL_TEXTURE_2D, glyph.texture);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

} // namespace gfx::detail
