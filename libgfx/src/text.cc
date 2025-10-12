#include "shaders.hh"
#include <detail/text.hh>
#include <detail/detail.hh>

namespace detail {

TextRenderer::TextRenderer(gfx::Window& window)
: m_window(window) {

    m_program = create_shader_program(shaders::vertex::text, shaders::fragment::text);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_position)));
    glEnableVertexAttribArray(a_pos);

    glGenBuffers(1, &m_uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(Vertex), m_uvs.data(), GL_STATIC_DRAW);

    GLint a_uv = glGetAttribLocation(m_program, "a_uv");
    glVertexAttribPointer(a_uv, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_uv);

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    if (FT_Init_FreeType(&m_ft) != 0) {
        throw std::runtime_error("failed to initialize ft2");
    }

    if (FT_New_Face(m_ft, "/usr/share/fonts/TTF/Roboto-Regular.ttf", 0, &m_face) != 0) {
        throw std::runtime_error("failed to load font");
    }


    // FT_Done_Face(m_face);
    // FT_Done_FreeType(m_ft);

}

void TextRenderer::draw(int x, int y, const char* text) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    FT_Set_Pixel_Sizes(m_face, 0, 100);

    if (FT_Load_Char(m_face, 'X', FT_LOAD_RENDER) != 0) {
        throw std::runtime_error("failed to load char");
    }

    unsigned int width = m_face->glyph->bitmap.width;
    unsigned int height = m_face->glyph->bitmap.rows;

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        m_face->glyph->bitmap.width,
        m_face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        m_face->glyph->bitmap.buffer
    );

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

}

} // namespace detail
