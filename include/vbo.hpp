#pragma once

#include "glad/glad.h"

namespace glrhi {
    class vbo {
    public:

        vbo(const void* data, unsigned int size);
        ~vbo();

        void bind();
        void unbind();

        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID;

    };
}