#include "glrhi/core/ssbo.hpp"
#include <iostream>

namespace glrhi {

    ssbo::ssbo(GLsizeiptr size, const void* data) {
        create(size, data);
    }

    ssbo::~ssbo() {
        glDeleteBuffers(1, &m_ID);
    }

    void ssbo::create(GLsizeiptr size, const void* data) {
        if (m_ID) {
            std::cerr << "ERROR::SSBO::BUFFER_ALREADY_CREATED\n";
            return;
        }

        glCreateBuffers(1, &m_ID);

        // Allocate memory on the GPU and optionally load data
        glNamedBufferStorage(m_ID, size, data, GL_DYNAMIC_STORAGE_BIT);
    }

    void ssbo::sendData(GLintptr offset, GLsizeiptr size, const void* data) const {
        glNamedBufferSubData(m_ID, offset, size, data);
    }

    void ssbo::addBindingPoint(GLuint bindingPoint) const {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_ID);
    }

}