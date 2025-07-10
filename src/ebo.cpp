#include "glrhi/core/ebo.hpp"

namespace glrhi {

	ebo::ebo(const void* data, unsigned int size) {
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, size, data, GL_STATIC_DRAW);
	}
	
	ebo::~ebo() {
		glDeleteBuffers(1, &m_ID);
	}

	void ebo::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void ebo::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}