#pragma once

#include "glad/glad.h"
#include "glrhi/shader.hpp"

#include <stb_image.h>

namespace glrhi {
    class texture {
    public:

        texture();
        ~texture();

        void loadEmpty(unsigned int width, unsigned int height, GLenum internalFormat) const;
        void loadData(unsigned int width, unsigned int height, GLenum internalFormat, GLenum dataFormat, GLenum type, const void* data) const;
        void loadFile(const char* path, GLenum internalFormat) const;

        void bind(GLuint textureUnit, glrhi::shader& shader, const char* textureUniform) const;
        void unbind() const;

    private:

        GLuint m_ID; 
    };
}