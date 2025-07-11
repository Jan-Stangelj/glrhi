#pragma once

#include "glad/glad.h"
#include "glrhi/core/shader.hpp"

#include "stb_image.h"

namespace glrhi {
    class texture2D {
    public:

        texture2D(GLsizei width, GLsizei height, GLenum internalFormat, GLsizei mips = 1);
        texture2D(const char* file, GLenum internalFormat, GLsizei mips = 1);
        ~texture2D();

        void loadData(GLenum dataFormat, GLenum dataType, const void* data, GLint mip = 0) const;

        void genMipmaps();

        void bind(GLuint textureUnit, glrhi::shader& shader, const char* textureUniform) const;

        GLuint getID() const { return m_ID; };

    private:

        GLuint m_ID;

        GLsizei m_width, m_height;
    };
}