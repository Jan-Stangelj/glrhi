/**
 * @file ubo.hpp
 * @brief Handles the creation and usage of Uniform Buffer Objects.
 */

#pragma once

#include <glad/glad.h>

namespace glrhi {

    /**
     * @brief Handles the creation and usage of Uniform Buffer Objects.
     */
    class ubo {
    public:

        /**
         * @brief Construct a new UBO.
         * 
         * @param size The size of the UBO in bytes.
         * @param data Optionally pass in all data on creation.
         */
        ubo(GLsizeiptr size, const void* data = nullptr);
        ~ubo();

        /**
         * @brief Uploads a chunk of data to the UBO one the GPU.
         * 
         * @param offset Byte offset in the SSBO, where data upload should begin.
         * @param size Size in bytes of the data to be uploaded.
         * @param data Pointer to the data in CPU memory to upload.
         */
        void sendData(GLintptr offset, GLsizeiptr size, const void* data) const;

        /**
         * @brief Adds the UBO to a binding point to be used in a shader.
         * 
         * @param bindingPoint The binding point to add the SSBO to.
         */
        void addBindingPoint(GLuint bindingPoint) const;

        /**
         * @brief Returns the UBO's OpenGL ID.
         * 
         * @return The UBO's OpenGL ID.
         */
        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID = 0;

    };
}