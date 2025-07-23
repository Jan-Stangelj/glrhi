#include "glrhi/core/ssbo.hpp"

namespace glrhi {

    ssbo::ssbo(GLsizeiptr size, const void* data) {
        glCreateBuffers(1, &m_ID); // gen buffer
        glNamedBufferStorage(m_ID, size, data, GL_DYNAMIC_STORAGE_BIT); // allocate memory
    }

    ssbo::~ssbo() {
        glDeleteBuffers(1, &m_ID); // delete buffer
    }

    void ssbo::sendData(GLintptr offset, GLsizeiptr size, const void* data) const {
        glNamedBufferSubData(m_ID, offset, size, &data); // send the data
    }

    void ssbo::addBindingPoint(GLuint bindingPoint) const {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_ID); // add binding point
    }

}