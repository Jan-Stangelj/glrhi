/**
 * @file vbo.hpp
 * @brief Handles the creation and usage of Vertex Buffer Objects.
 */

#pragma once

#include <glad/glad.h>

namespace glrhi {

    /**
     * @brief Handles the creation and usage of Vertex Buffer Objects.
     */
    class vbo {
    public:

        /**
         * @brief Construct a new VBO.
         * 
         * @param data Pointer to the vertices data.
         * @param size Size of the data in bytes.
         */
        vbo(const void* data, unsigned int size);
        ~vbo();

        /**
         * @brief Returns The VBO's OpenGL ID.
         * 
         * @return The VBO's OpenGL ID.
         */
        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID = 0;
        
    };
}