#include "glrhi/core/vao.hpp"

namespace glrhi {

    vao::vao() {
        glCreateVertexArrays(1, &m_ID);
    }

    vao::~vao() {
        if (m_ID) glDeleteVertexArrays(1, &m_ID);
    }

    void vao::addAttribute(GLint size, GLenum type, GLboolean normalized, GLuint pointer) {
        if (!m_ID) return;

        // Activate the vertex attribute
        glEnableVertexArrayAttrib(m_ID, m_attributeCounter);
        glVertexArrayAttribBinding(m_ID, m_attributeCounter, 0);

        // Set the vertex attribute
        glVertexArrayAttribFormat(m_ID, m_attributeCounter, size, type, normalized, pointer);
        m_attributeCounter++;
    }

    void vao::init(const vbo& VBO, const glrhi::ebo& EBO, GLsizei stride) const {
        if (!m_ID) return;

        // Link the VBO to the VAO
        glVertexArrayVertexBuffer(m_ID, 0, VBO.getID(), 0, stride);

        // Link the EBO to the VAO
        glVertexArrayElementBuffer(m_ID, EBO.getID());
    }

    void vao::bind() const {
        if (m_ID) glBindVertexArray(m_ID);
    }

    void vao::unbind() const {
        if (m_ID) glBindVertexArray(0);
    }
}
