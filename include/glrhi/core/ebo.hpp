/**
 * @file ebo.hpp
 * @brief Handles creation and usage of Element Buffer Objects.
 */

#pragma once

#include <glad/glad.h>

namespace glrhi {

    /**
     * @brief Handles creation and usage of Element Buffer Objects.
     */
    class ebo {
    public:

        /**
         * @brief Construct a new EBO.
         * 
         * @param data Pointer to the indices data.
         * @param size Size of the data in bytes.
         */
        ebo(const void* data, unsigned int size);
        ebo() = default;
        ~ebo();

        /**
         * @brief Construct a new EBO.
         * 
         * @param data Pointer to the indices data.
         * @param size Size of the data in bytes.
         */
        void create(const void* data, unsigned int size);
        
        /**
         * @brief Returns the EBO's OpenGL ID.
         * 
         * @return The EBO's OpenGL ID.
         */
        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID = 0;

    };
}
