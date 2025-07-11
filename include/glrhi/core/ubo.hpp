#pragma once

#include <glad/glad.h>

namespace glrhi {
    class ubo {
    public:

        ubo(GLsizeiptr size, const void* data = nullptr);
        ~ubo();

        void sendData(GLintptr offset, GLsizeiptr size, const void* data) const;

        void addBindingPoint(GLuint bindingPoint) const;
        void resetBindingPoint(GLuint bindingPoint) const;

        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID = 0;

    };
}