#pragma once

#include <glad/glad.h>

#include "glrhi/core/vbo.hpp"
#include "glrhi/core/ebo.hpp"

#include <vector>

namespace glrhi {
    class vao {
    public:

        vao();
        ~vao();

        void addAttribute(GLint size, GLenum type, GLboolean normalized, GLuint pointer);
        void init(const glrhi::vbo& VBO, const glrhi::ebo& EBO, GLsizei stride) const;

        void bind() const;
        void unbind() const;

        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID;

        unsigned int m_attributeCounter = 0;

    };
}