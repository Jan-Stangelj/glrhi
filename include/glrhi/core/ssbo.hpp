/**
 * @file ssbo.hpp
 * @brief Handles the creation and usage of Shader Storage Buffer Objects.
 */

#pragma once

#include <glad/glad.h>

namespace glrhi {

    /**
     * @brief Handles the creation and usage of Shader Storage Buffer Objects.
     */
    class ssbo {
    public:

        /**
         * @brief Construct a new SSBO.
         * 
         * @param size The size of the SSBO in bytes.
         * @param data Optionally pass in all data on creation.
         */
        ssbo(GLsizeiptr size, const void* data = nullptr);
        ~ssbo();

        /**
         * @brief Uploads a chunk of data to the SSBO on the GPU.
         * 
         * @param offset Byte offset in the SSBO, where data upload should begin.
         * @param size Size in bytes of the data to be uploaded.
         * @param data Pointer to the data in CPU memory to upload.
         */
        void sendData(GLintptr offset, GLsizeiptr size, const void* data) const;

        /**
         * @brief Adds the SSBO to a binding point to be used in a shader.
         * 
         * @param bindingPoint The binding point to add the SSBO to.
         */
        void addBindingPoint(GLuint bindingPoint) const;

        /**
         * @brief Returns the SSBO's OpenGL ID.
         * 
         * @return The SSBO's OpenGL ID.
         */
        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID = 0;

    };
}