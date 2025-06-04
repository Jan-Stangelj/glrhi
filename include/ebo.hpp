#pragma once

#include "glad/glad.h"

namespace glrhi {
    class ebo {
    public:

        ebo(const void* data, unsigned int size);
        ~ebo();

        void bind();
        void unbind();

    private:

        GLuint m_ID;

    };
}