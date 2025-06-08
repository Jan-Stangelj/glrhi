#include "glrhi/core/vbo.hpp"

glrhi::vbo::vbo(const void* data, unsigned int size) {
	glCreateBuffers(1, &m_ID);
    glNamedBufferData(m_ID, size, data, GL_STATIC_DRAW);
}
glrhi::vbo::~vbo() {
	glDeleteBuffers(1, &m_ID);
}

void glrhi::vbo::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}
void glrhi::vbo::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}