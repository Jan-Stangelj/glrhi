#include "glrhi/core/vbo.hpp"
#include <iostream>

namespace glrhi {

	vbo::vbo(const void* data, unsigned int size) {
		create(data, size);
	}
	vbo::~vbo() {
		glDeleteBuffers(1, &m_ID);
	}

	void vbo::create(const void* data, unsigned int size) {
		if (m_ID) {
			std::cerr << "ERROR::VBO::BUFFER_ALREADY_CREATED\n";
			return;
		}

		glCreateBuffers(1, &m_ID);

 		// Allocate memory on the GPU
		glNamedBufferData(m_ID, size, data, GL_STATIC_DRAW);
	}
}
