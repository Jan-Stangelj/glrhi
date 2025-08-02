#include "glrhi/core/vao.hpp"

namespace glrhi {

    vao::vao() {
        glCreateVertexArrays(1, &m_ID);
    }

    vao::~vao() {
        glDeleteVertexArrays(1, &m_ID);
    }

    void vao::addAttribute(GLint size, GLenum type, GLboolean normalized, GLuint pointer) {
        // Activate the vertex attribute
        glEnableVertexArrayAttrib(m_ID, m_attributeCounter);
        glVertexArrayAttribBinding(m_ID, m_attributeCounter, 0);

        // Set the vertex attribute
        glVertexArrayAttribFormat(m_ID, m_attributeCounter, size, type, normalized, pointer);
        m_attributeCounter++;
    }

    void vao::init(const vbo& VBO, const glrhi::ebo& EBO, GLsizei stride) const {
        // Link the VBO to the VAO
        glVertexArrayVertexBuffer(m_ID, 0, VBO.getID(), 0, stride);

        // Link the EBO to the VAO
        glVertexArrayElementBuffer(m_ID, EBO.getID());
    }

    void vao::bind() const {
        glBindVertexArray(m_ID);
    }

    void vao::unbind() const {
        glBindVertexArray(0);
    }
}
