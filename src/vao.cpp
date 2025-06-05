#include "vao.hpp"

glrhi::vao::vao() {
    glCreateVertexArrays(1, &m_ID);
}

glrhi::vao::~vao() {
    glDeleteVertexArrays(1, &m_ID);
}

void glrhi::vao::addAttribute(GLint size, GLenum type, GLboolean normalized, GLuint pointer) {
    glEnableVertexArrayAttrib(m_ID, m_attributeCounter);
    glVertexArrayAttribBinding(m_ID, m_attributeCounter, 0);
    glVertexArrayAttribFormat(m_ID, m_attributeCounter, size, type, normalized, pointer);
    m_attributeCounter++;
}

void glrhi::vao::init(const glrhi::vbo& VBO, const glrhi::ebo& EBO, GLsizei stride) {
    glVertexArrayVertexBuffer(m_ID, 0, VBO.getID(), 0, stride);
    glVertexArrayElementBuffer(m_ID, EBO.getID());
}

void glrhi::vao::bind() {
    glBindVertexArray(m_ID);
}

void glrhi::vao::unbind() {
    glBindVertexArray(0);
}