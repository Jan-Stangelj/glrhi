#include "vbo.hpp"

glrhi::vbo::vbo(const void* data, unsigned int size) {
	glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
glrhi::vbo::~vbo() {
	glDeleteBuffers(1, &m_ID);
}

void glrhi::vbo::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}
void glrhi::vbo::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}