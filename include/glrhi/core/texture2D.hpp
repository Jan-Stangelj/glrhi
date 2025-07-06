/**
 * @file texture2D.hpp
 * @brief Texture2D class definition.
 */

#pragma once

#include "glad/glad.h"
#include "glrhi/core/shader.hpp"

#include "stb_image.h"

namespace glrhi {

    /**
     * @brief Represents a 2D texture.
     */
    class texture2D {
    public:

        /**
         * @brief Constructs a texture2D object.
         * @param width The width of the texture.
         * @param height The height of the texture.
         * @param internalFormat The internal format of the texture.
         * @param mips The number of mipmap levels. Defaults to 1.
         */
        texture2D(GLsizei width, GLsizei height, GLenum internalFormat, GLsizei mips = 1);

        /**
         * @brief Constructs a texture2D object from a file.
         * @param file The path to the image file.
         * @param internalFormat The internal format of the texture.
         * @param mips The number of mipmap levels. Defaults to 1.
         */
        texture2D(const char* file, GLenum internalFormat, GLsizei mips = 1);
        
        /**
         * @brief Destructor for texture2D.
         */
        ~texture2D();


        /**
         * @brief Loads image data to the GPU.
         * @param dataFormat The data format for the texture.
         * @param dataType The data type for the texture.
         * @param data Pointer to the image data.
         * @param mip The mipmap level.
         */
        void loadData(GLenum dataFormat, GLenum dataType, const void* data, GLint mip = 0) const;

        /**
         * @brief Generates mipmaps for the texture.
         */
        void genMipmaps();

        /**
         * @brief Binds the texture to a texture unit and sets the uniform.
         * @param textureUnit The texture unit to bind to.
         * @param shader The shader to set the uniform on.
         * @param textureUniform The name of the texture uniform.
         */
        void bind(GLuint textureUnit, glrhi::shader& shader, const char* textureUniform) const;

        /**
         * @brief Gets the texture ID.
         * @return The texture ID.
         */
        GLuint getID() const { return m_ID; };

    private:

        GLuint m_ID;

        GLsizei m_width, m_height;
    };
}