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

        ebo(const glrhi::ebo&) = delete;
        glrhi::ebo& operator=(const glrhi::ebo&) = delete;

        ebo(glrhi::ebo&& other) noexcept {
            m_ID = other.m_ID;
            other.m_ID = 0;
        }
        glrhi::ebo& operator=(glrhi::ebo&& other) {
            if (this != &other) {
                if (m_ID)
                    glDeleteBuffers(1, &m_ID);
                m_ID = other.m_ID;
                other.m_ID = 0;
            }
            return *this;
        }

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
