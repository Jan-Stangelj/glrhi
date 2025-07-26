#include "glrhi/core/ubo.hpp"

namespace glrhi {

    ubo::ubo(GLsizeiptr size, const void* data) {
        glCreateBuffers(1, &m_ID);

        // Allocate memory on the GPU and optionally load data
        glNamedBufferData(m_ID, size, data, GL_DYNAMIC_DRAW);
    }

    ubo::~ubo() {
        glDeleteBuffers(1, &m_ID);
    }

    void ubo::sendData(GLintptr offset, GLsizeiptr size, const void* data) const {
        glNamedBufferSubData(m_ID, offset, size, &data);
    }

    void ubo::addBindingPoint(GLuint bindingPoint) const {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_ID);
    }

}