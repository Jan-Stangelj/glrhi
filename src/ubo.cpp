#include "glrhi/core/ubo.hpp"

namespace glrhi {

    ubo::ubo(GLsizeiptr size, const void* data) {
        glCreateBuffers(1, &m_ID); // gen buffer
        glNamedBufferData(m_ID, size, data, GL_STATIC_DRAW); // allocate memory
    }

    ubo::~ubo() {
        glDeleteBuffers(1, &m_ID); // delete buffer
    }

    void ubo::sendData(GLintptr offset, GLsizeiptr size, const void* data) const {
        glNamedBufferSubData(m_ID, offset, size, &data); // send the data
    }

    void ubo::addBindingPoint(GLuint bindingPoint) const {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_ID); // add binding point
    }

}