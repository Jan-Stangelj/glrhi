#pragma once

#include <glad/glad.h>

namespace glrhi {
    class ssbo {
    public:

        ssbo(GLsizeiptr size, const void* data = nullptr);
        ~ssbo();

        void sendData(GLintptr offset, GLsizeiptr size, const void* data) const;

        void addBindingPoint(GLuint bindingPoint) const;

        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID = 0;

    };
}