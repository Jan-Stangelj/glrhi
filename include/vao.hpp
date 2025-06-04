#pragma once

#include "glad/glad.h"

#include <vector>

namespace glrhi {
    class vao {
    public:

        vao();
        ~vao();

        //                       index         size        type            normalized          stride        (void*)0
        void addAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, void* pointer);

        void bind();
        void unbind();

    private:

        GLuint m_ID;

        unsigned int m_attributeCounter = 0;

    };
}