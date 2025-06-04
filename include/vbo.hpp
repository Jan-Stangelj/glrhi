#pragma once

#include "glad/glad.h"

namespace glrhi {
    class vbo {
    public:

        vbo(const void* data, unsigned int size);
        ~vbo();

        void bind();
        void unbind();

    private:

        GLuint m_ID;

    };
}