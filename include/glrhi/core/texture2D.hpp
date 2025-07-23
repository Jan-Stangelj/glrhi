#pragma once

#include "glad/glad.h"

#include "stb_image.h"

#include <filesystem>

namespace glrhi {
    class texture2D {
    public:

        texture2D(GLsizei width, GLsizei height, GLenum internalFormat, GLsizei mips = 1);
        texture2D(const std::filesystem::path& file, GLenum internalFormat, GLsizei mips = 1);
        ~texture2D();

        void loadData(GLenum dataFormat, GLenum dataType, const void* data, GLint mip = 0) const;

        /// @brief Generates mipmaps using default OpenGL function.
        void genMipmaps() const;

        /// @warning Only binds texture unit, not the texture.
        void bind(GLuint textureUnit);

        /// @warning Only binds texture unit, not the texture.
        void bindImage(GLuint unit, GLint mip);

        /// @brief Gets the handle for bindless usage.
        GLuint64 getSamplerHandle();

        /// @brief Gets the handle for bindless image usage. Acces defaults to read/write.
        GLuint64 getImageHandle(GLint mip);

        GLuint getID() const { return m_ID; };

    private:

        GLuint m_ID = 0;

        GLuint64 samplerHandle = 0;
        GLuint64 imageHandle = 0;

        GLsizei m_width, m_height;
        GLenum m_format;
    };
}