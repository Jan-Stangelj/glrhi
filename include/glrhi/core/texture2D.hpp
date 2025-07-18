#pragma once

#include "glad/glad.h"

#include "stb_image.h"

namespace glrhi {
    class texture2D {
    public:

        texture2D(GLsizei width, GLsizei height, GLenum internalFormat, GLsizei mips = 1);
        texture2D(const char* file, GLenum internalFormat, GLsizei mips = 1);
        ~texture2D();

        void loadData(GLenum dataFormat, GLenum dataType, const void* data, GLint mip = 0) const;

        void genMipmaps() const;

        GLuint64 getSamplerHandle();
        GLuint64 getImageHandle();

        GLuint getID() const { return m_ID; };

    private:

        GLuint m_ID = 0;

        GLuint64 samplerHandle = 0;
        GLuint64 imageHandle = 0;

        GLsizei m_width, m_height;
        GLenum m_format;
    };
}