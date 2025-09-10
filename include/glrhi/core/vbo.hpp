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
        vbo() = default;
        ~vbo();

        vbo(const glrhi::vbo&) = delete;
        glrhi::vbo& operator=(const glrhi::vbo&) = delete;

        vbo(glrhi::vbo&& other) noexcept {
            m_ID = other.m_ID;
            other.m_ID = 0;
        }
        glrhi::vbo& operator=(glrhi::vbo&& other) {
            if (this != &other) {
                if (m_ID)
                    glDeleteBuffers(1, &m_ID);
                m_ID = other.m_ID;

                other.m_ID = 0;
            }
            return *this;
        }

        /**
         * @brief Construct a new VBO.
         * 
         * @param data Pointer to the vertices data.
         * @param size Size of the data in bytes.
         */
        void create(const void* data, unsigned int size);

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