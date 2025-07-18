#pragma once

#include <glad/glad.h>

namespace glrhi {
    class ebo {
    public:

        ebo(const void* data, unsigned int size);
        ~ebo();

        void bind() const;
        void unbind() const;
        
        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID;

    };
}
