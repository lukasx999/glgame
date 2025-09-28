#include <detail/triangle.hh>
#include "shaders.hh"

detail::TriangleRenderer::TriangleRenderer(GLFWwindow* window)
: m_window(window)
{

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(TriangleVertex), m_vertices.data(), GL_STATIC_DRAW);

    m_program = create_shader_program(triangle_shader_vertex_src, shader_fragment_src);

    GLint a_position = glGetAttribLocation(m_program, "a_position");
    glVertexAttribPointer(a_position, 2, GL_FLOAT, false, sizeof(TriangleVertex), reinterpret_cast<void*>(offsetof(TriangleVertex, m_position)));
    glEnableVertexAttribArray(a_position);

    GLint a_vertex_no = glGetAttribLocation(m_program, "a_vertex_no");
    glVertexAttribPointer(a_vertex_no, 1, GL_INT, false, sizeof(TriangleVertex), reinterpret_cast<void*>(offsetof(TriangleVertex, m_vertex_no)));
    glEnableVertexAttribArray(a_vertex_no);

    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}
