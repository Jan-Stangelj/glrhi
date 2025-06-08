#pragma once

#include "glad/glad.h"
#include "glrhi/core/shader.hpp"

#include <stb_image.h>

namespace glrhi {
    class texture2D {
    public:

        texture2D();
        ~texture2D();

        void loadEmpty(unsigned int width, unsigned int height, GLenum internalFormat) const;
        void loadData(unsigned int width, unsigned int height, GLenum internalFormat, GLenum dataFormat, GLenum type, const void* data) const;
        void loadFile(const char* path, GLenum internalFormat) const;
        void loadFileFloat(const char* path, GLenum internalFormat) const;

        void bind(GLuint textureUnit, glrhi::shader& shader, const char* textureUniform) const;
        void unbind() const;

    private:

        GLuint m_ID; 
    };
}