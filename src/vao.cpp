#include "vao.hpp"

glrhi::vao::vao() {
    glGenVertexArrays(1, &m_ID);
}

glrhi::vao::~vao() {
    glDeleteVertexArrays(1, &m_ID);
}

void glrhi::vao::addAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, void* pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer); // index size type normalized stride (void*)0
    glEnableVertexAttribArray(m_attributeCounter);
    m_attributeCounter++;
}

void glrhi::vao::bind() {
    glBindVertexArray(m_ID);
}

void glrhi::vao::unbind() {
    glBindVertexArray(0);
}