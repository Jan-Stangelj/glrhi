#include "glrhi/core/ebo.hpp"
#include <iostream>

namespace glrhi {

	ebo::ebo(const void* data, unsigned int size) {
		create(data, size);
	}
	
	ebo::~ebo() {
		glDeleteBuffers(1, &m_ID);
	}

	void ebo::create(const void* data, unsigned int size) {
		if (m_ID) {
			std::cerr << "ERROR::EBO::BUFFER_ALREADY_CREATED\n";
			return;
		}

		glCreateBuffers(1, &m_ID);

		// Allocate memory on the GPU and load data
		glNamedBufferData(m_ID, size, data, GL_STATIC_DRAW);
	}

}