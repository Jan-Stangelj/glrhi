/**
 * @file texture2D.hpp
 * @brief Handles the creation and usage of 2d textures.
 */

#pragma once

#include <glad/glad.h>

#include <stb_image.h>

#include <filesystem>

namespace glrhi {

    /**
     * @brief Handles the creation and usage of 2d textures.
     */
    class texture2D {
    public:

        /**
         * @brief Construct a new texture2D.
         * 
         * @param width Width of the texture.
         * @param height Height of the texture.
         * @param internalFormat The internal format of the texture (ex. GL_RGBA32F)
         * @param mips The number of mips the texture should have.
         */
        texture2D(GLsizei width, GLsizei height, GLenum internalFormat);

        /**
         * @brief Construct a new texture2D.
         * 
         * @param file Path to the file from which to load the texture.
         * @param internalFormat The internal format of the texture (ex. GL_RGBA32F)
         * @param mips The number of mips the texture should have.
         */
        texture2D(const std::filesystem::path& file, GLenum internalFormat);
        texture2D() = default;
        ~texture2D();

        texture2D(const glrhi::texture2D&) = delete;
        glrhi::texture2D& operator=(const glrhi::texture2D&) = delete;

        texture2D(glrhi::texture2D&& other) noexcept {
            m_ID = other.m_ID;
            other.m_ID = 0;
        }
        glrhi::texture2D& operator=(glrhi::texture2D&& other) {
            if (this != &other) {
                if (m_ID) {
                    // Make handles non resident if they were used
                    if (m_samplerHandle)
                        glMakeTextureHandleNonResidentARB(m_samplerHandle);

                    if (m_imageHandle)
                        glMakeImageHandleNonResidentARB(m_imageHandle);

                    // Delete the texture buffer
                    glDeleteTextures(1, &m_ID);
                }
                m_ID = other.m_ID;
                m_samplerHandle = other.m_samplerHandle;
                m_imageHandle = other.m_imageHandle;
                m_format = other.m_format;
                m_width = other.m_width;
                m_height = other.m_height;

                other.m_ID = 0;
            }
            return *this;
        }

        /**
         * @brief Construct a new texture2D.
         * 
         * @param width Width of the texture.
         * @param height Height of the texture.
         * @param internalFormat The internal format of the texture (ex. GL_RGBA32F)
         * @param mips The number of mips the texture should have.
         */
        void create(GLsizei width, GLsizei height, GLenum internalFormat);

        /**
         * @brief Construct a new texture2D.
         * 
         * @param file Path to the file from which to load the texture.
         * @param internalFormat The internal format of the texture (ex. GL_RGBA32F)
         * @param mips The number of mips the texture should have.
         */
        void create(const std::filesystem::path& file, GLenum internalFormat);

        /**
         * @brief Loads data into the texture.
         * 
         * @param dataFormat The format of input data (eg. GL_RGBA).
         * @param dataType The type of input data (eg. GL_BYTE).
         * @param data Pointer to the data on the CPU.
         * @param mip To which mip to upload the data.
         */
        void loadData(GLenum dataFormat, GLenum dataType, const void* data, GLint mip = 0) const;
        
        /**
         * @brief Generates mipmaps using default OpenGL function.
         */
        void genMipmaps() const;

        /**
         * @brief Binds the texture to a texture unit.
         * 
         * @param textureUnit The texture unit to bind to.
         */
        void bind(GLuint textureUnit) const;

        /**
         * @brief Binds the texture as a image.
         * 
         * @param unit Image unit to bind to.
         * @param mip The mip to bind.
         */
        void bindImage(GLuint unit, GLint mip) const;

        /**
         * @brief Get the texture handle for bindless texture use.
         * 
         * @return The texture handle.
         */
        GLuint64 getSamplerHandle();

        /**
         * @brief Get the texture handle for bindless image use. Acces defaults to read/write.
         * 
         * @param mip The mip to get the handle for.
         * @return The image handle.
         */
        GLuint64 getImageHandle(GLint mip);

        /**
         * @brief Returns the texture's OpenGL ID.
         * 
         * @return The texture's OpenGL ID.
         */
        GLuint getID() const { return m_ID; };

    private:

        GLuint m_ID = 0;

        GLuint64 m_samplerHandle = 0;
        GLuint64 m_imageHandle = 0;

        GLsizei m_width = 1024, m_height = 1024;
        GLenum m_format = GL_RGBA8;
    };
}