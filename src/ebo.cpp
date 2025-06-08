#include "glrhi/core/ebo.hpp"

glrhi::ebo::ebo(const void* data, unsigned int size) {
    glCreateBuffers(1, &m_ID);
    glNamedBufferData(m_ID, size, data, GL_STATIC_DRAW);
}
glrhi::ebo::~ebo() {
	glDeleteBuffers(1, &m_ID);
}

void glrhi::ebo::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}
void glrhi::ebo::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}